package com.gitlab.ffaero.tools.spacepi.support.debugclient;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class FormatFileReader {

	private BufferedReader reader;

	public FormatFileReader(File file) throws IOException {
		this(new BufferedReader(new FileReader(file)));
	}

	public FormatFileReader(BufferedReader reader) throws IOException {
		this.reader = reader;
	}

	public List<Format> read() throws IOException {
		List<Format> formats = new ArrayList<Format>();
		Format curFormat = null;
		while (reader.ready()) {
			String line = reader.readLine();
			if (line.trim().startsWith("//") || line.length() == 0) {
				continue;
			}
			String[] linedata = line.split(":", 2);
			if (linedata.length != 2) {
				throw new IllegalStateException("Invalid format file format on line \"" + line + "\"");
			}
			if (linedata[0].equals("TOPIC")) {
				if (curFormat != null) {
					formats.add(curFormat);
					curFormat = null;
				}
				curFormat = new Format(linedata[1], new ArrayList<String>(), new ArrayList<DataType>(), new ArrayList<Integer>());
			} else if (curFormat != null) {
				try {
					String[] typedata = linedata[0].split("@", 2);
					DataType type = DataType.valueOf(typedata[0]);
					curFormat.datatypes.add(type);
					if (typedata.length > 1) {
						curFormat.lengths.add(Integer.parseInt(typedata[1]));
					} else {
						curFormat.lengths.add(0);
					}
				} catch (IllegalArgumentException e) {
					throw new IllegalStateException("Unknown data type \"" + linedata[0] + "\"");
				}

				curFormat.labels.add(linedata[1]);
			} else {
				throw new IllegalStateException(
						"Attempt to define format field without defining format name \"" + line + "\"");
			}

		}
		if (curFormat != null) {
			formats.add(curFormat);
		}
		return formats;
	}

	public static final class Format {
		private String name;
		private List<String> labels;
		private List<DataType> datatypes;
		private List<Integer> lengths;

		public Format(String name, List<String> labels, List<DataType> datatypes, List<Integer> lengths) {
			this.name = name;
			this.labels = labels;
			this.datatypes = datatypes;
			this.lengths = lengths;
		}

		public String getName() {
			return name;
		}

		public void setName(String name) {
			this.name = name;
		}

		public List<String> getLabels() {
			return labels;
		}

		public void setLabels(List<String> labels) {
			this.labels = labels;
		}

		public List<DataType> getDataTypes() {
			return datatypes;
		}

		public void setDataTypes(List<DataType> datatypes) {
			this.datatypes = datatypes;
		}

		public List<Integer> getLengths() {
			return lengths;
		}

		public void setLengths(List<Integer> lengths) {
			this.lengths = lengths;
		}

		@Override
		public String toString() {
			return name;
		}

	}

}
