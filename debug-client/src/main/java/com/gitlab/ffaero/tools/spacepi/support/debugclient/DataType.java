package com.gitlab.ffaero.tools.spacepi.support.debugclient;

public enum DataType {

	UINT8(1), INT8(1), UINT16(2), INT16(2), UINT32(4), INT32(4), UINT64(8), INT64(8), FLOAT32(4), FLOAT64(8),
	ASCII_STRING(-1), ASCII_STRING_NT(-1);

	private int size;

	private DataType(int size) {
		this.size = size;
	}

	public int getSize() {
		return size;
	}

}
