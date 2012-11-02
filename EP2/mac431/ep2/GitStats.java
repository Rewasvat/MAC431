package mac431.ep2;

import java.io.IOException;
import java.util.*;
        
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.conf.*;
import org.apache.hadoop.io.*;
import org.apache.hadoop.mapred.*;
import org.apache.hadoop.util.*;

/* git log --numstat --pretty=format:"%H %ae"  */
        
public class GitStats {
        
 public static class Map extends MapReduceBase implements Mapper<Text, Text, Text, DoubleWritable> {
    private final static DoubleWritable one = new DoubleWritable(1);
        
    public void map(Text key, Text value, OutputCollector<Text, DoubleWritable> output, Reporter reporter) throws IOException {
        String data = value.toString();
        StringTokenizer lineTokenizer = new StringTokenizer(data, "\n\r");
        
        String currentHash = "";
        String currentAuthor = "";
        int linesAdd, linesRemoved;
        int totalLinesAdd = 0, totalLinesRemoved = 0;
        String filepath;
        while (lineTokenizer.hasMoreTokens()) {
        	String line = lineTokenizer.nextToken();
        	StringTokenizer tokenizer = new StringTokenizer(line, " \t");
        	int tokens = tokenizer.countTokens();
        	if (tokens == 2) {
        		if (!currentAuthor.isEmpty()) {
        			int sum = totalLinesAdd + totalLinesRemoved;
        			output.collect(new Text("modlinesauthor_"+currentAuthor), new DoubleWritable(sum));
        			output.collect(new Text("modlinescommit_"+currentHash), new DoubleWritable(sum));
        			output.collect(new Text("numcommits_"+currentAuthor), one);
        		}
        		
        		/*reading commit info ( hash , authoremail ) */
        		currentHash = tokenizer.nextToken();
        		currentAuthor = tokenizer.nextToken();
        		totalLinesAdd = totalLinesRemoved = 0;
        	}
        	else if (tokens == 3) {
        		/*reading commit line info ( linesAdd, linesRemoved, file ) */
        		String ladd = tokenizer.nextToken();
        		String lrem = tokenizer.nextToken();
        		if (!ladd.equals("-"))
        			linesAdd = Integer.parseInt(ladd);
        		else
        			linesAdd = 0;
        		totalLinesAdd += linesAdd;
        		
        		if (!lrem.equals("-"))
        			linesRemoved = Integer.parseInt(lrem);
        		else
        			linesRemoved = 0;
        		totalLinesRemoved += linesRemoved;
        		
        		filepath = tokenizer.nextToken();
        		output.collect(new Text("modlinesfile_"+filepath), new DoubleWritable(linesAdd + linesRemoved));
        	}
        }
		if (!currentAuthor.isEmpty()) {
			int sum = totalLinesAdd + totalLinesRemoved;
			output.collect(new Text("modlinesauthor_"+currentAuthor), new DoubleWritable(sum));
			output.collect(new Text("modlinescommit_"+currentHash), new DoubleWritable(sum));
			output.collect(new Text("numcommits_"+currentAuthor), one);
		}
    }
 } 
        
 public static class Reduce extends MapReduceBase implements Reducer<Text, DoubleWritable, Text, DoubleWritable> {

    public void reduce(Text key, Iterator<DoubleWritable> values, OutputCollector<Text, DoubleWritable> output, Reporter reporter) throws IOException {
        int n = 0;
        double mean = 0, M2 = 0, delta;
        double sum = 0, variance, stddev;
        double value;
        
        if(key.toString().startsWith("total_") || key.toString().startsWith("mean_")
        		|| key.toString().startsWith("stddev_")) {
        	output.collect(key, values.next());
        	return;
        }
        
        while (values.hasNext()) {
        	value = values.next().get();
            sum += value;
            n = n + 1;
            delta = value - mean;
            mean = mean + delta/n;
            M2 = M2 + delta*(value - mean);
        }
        variance = M2/(n - 1);
        stddev = Math.sqrt(variance);
        output.collect(new Text("total_" + key.toString()), new DoubleWritable(sum));
        output.collect(new Text("mean_" + key.toString()), new DoubleWritable(mean));
        output.collect(new Text("stddev_" + key.toString()), new DoubleWritable(stddev));
    }
 }
        
 public static void main(String[] args) throws Exception {
    JobConf conf = new JobConf(GitStats.class);
    conf.setJobName("gitstats");
        
    conf.setOutputKeyClass(Text.class);
    conf.setOutputValueClass(DoubleWritable.class);
        
    conf.setMapperClass(Map.class);
    conf.setCombinerClass(Reduce.class);
    conf.setReducerClass(Reduce.class);
        
    //conf.setInputFormat(TextInputFormat.class);
    conf.setInputFormat(CommitInputFormat.class);
    conf.setOutputFormat(TextOutputFormat.class);
        
    /* Deprecated ?
     * conf.setInputPath(new Path(args[0]));
     * conf.setOutputPath(new Path(args[1]));
     */

	System.out.println("Arg0 = "+args[0]);
	System.out.println("Arg1 = "+args[1]);
	CommitInputFormat.setInputPaths(conf, new Path(args[0]));
    FileOutputFormat.setOutputPath(conf, new Path(args[1]));
        
    JobClient.runJob(conf);
 }
        
}
