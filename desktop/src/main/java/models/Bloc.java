package models;

import com.google.gson.annotations.SerializedName;

public class Bloc {
    @SerializedName("id")
    private int id;
    @SerializedName("nonce")
    private int nonce;
    @SerializedName("hash")
    private String hash;
    @SerializedName("content")
    private String content;

    public Bloc(int id, int nonce, String hash, String content) {
        this.id = id;
        this.nonce = nonce;
        this.hash = hash;
        this.content = content;
    }

}
