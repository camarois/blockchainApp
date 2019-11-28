package models;

import com.google.gson.annotations.SerializedName;

public class BlocksRequest {
    @SerializedName("block-count")
    private int blockCount;
    @SerializedName("miner-id")
    private int minerId;

    BlocksRequest(int blockCount, int minerId) {
        this.blockCount = blockCount;
        this.minerId = minerId;
    }
}
