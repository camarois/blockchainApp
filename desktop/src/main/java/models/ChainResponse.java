package models;

import com.google.gson.annotations.SerializedName;

import java.util.ArrayList;

public class ChainResponse {
    @SerializedName("blocks")
    private ArrayList<Block> blocks;

    public ChainResponse(ArrayList<Block> blocks) {
        this.blocks = blocks;
    }

    public ArrayList<Block> getBlocks() {
        return blocks;
    }
}
