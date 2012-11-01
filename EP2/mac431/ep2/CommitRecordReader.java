package mac431.ep2;

import java.io.IOException;

import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapred.FileSplit;
import org.apache.hadoop.mapred.JobConf;
import org.apache.hadoop.mapred.LineRecordReader;
import org.apache.hadoop.mapred.RecordReader;

public class CommitRecordReader implements RecordReader<Text, Text> {
	private LineRecordReader lineReader;
	private LongWritable lineKey;
	private Text lineValue;
	private String nextCommit;

	public CommitRecordReader(JobConf job, FileSplit split) throws IOException {
		lineReader = new LineRecordReader(job, split);

		lineKey = lineReader.createKey();
		lineValue = lineReader.createValue();
		nextCommit = "";

		if (lineReader.next(lineKey, lineValue)) {
			nextCommit = lineValue.toString();
		}
	}

	public boolean next(Text key, Text value) throws IOException {
		// get the next line
		if (nextCommit.isEmpty()) {
			return false;
		}

		// parse the lineValue which is in the format:
		// POG POG POG
		String[] pieces = nextCommit.split(" ");
		if (pieces.length != 2) {
			throw new IOException("Invalid record received");
		}
		key.set(pieces[0].trim()); // commit hash is the output key.

		value.set(nextCommit + "\n");
		nextCommit = "";
		while (lineReader.next(lineKey, lineValue)) {
			String[] valuePieces = lineValue.toString().split(" ");
			if (valuePieces.length == 2) {
				nextCommit = lineValue.toString();
				break;
			}
			value.set(value.toString() + lineValue.toString() + "\n");
		}

		return true;
	}

	public Text createKey() {
		return new Text("");
	}

	public Text createValue() {
		return new Text();
	}

	public long getPos() throws IOException {
		return lineReader.getPos();
	}

	public void close() throws IOException {
		lineReader.close();
	}

	public float getProgress() throws IOException {
		return lineReader.getProgress();
	}

}
