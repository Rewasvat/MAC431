package mac431.ep2;

import java.io.IOException;
import java.util.*;

import org.apache.hadoop.io.*; //import org.apache.hadoop.mapred.*;
import org.apache.hadoop.mapred.FileInputFormat;
import org.apache.hadoop.mapred.FileSplit;
import org.apache.hadoop.mapred.RecordReader;
import org.apache.hadoop.mapred.InputSplit;
import org.apache.hadoop.mapred.JobConf;
import org.apache.hadoop.mapred.Reporter;

public class CommitInputFormat extends FileInputFormat<Text, Text> {

	public RecordReader<Text, Text> getRecordReader(InputSplit input,
			JobConf job, Reporter reporter) throws IOException {

		reporter.setStatus(input.toString());
		return new CommitRecordReader(job, (FileSplit) input);
	}
}
