package models;

import com.google.gson.annotations.SerializedName;

public class ChainRequest {
    @SerializedName("dernier_blocs")
    private int blockCount;

    public ChainRequest(int blockCount) {
        this.blockCount = blockCount;
    }
}
