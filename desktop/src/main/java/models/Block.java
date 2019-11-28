package models;

import com.google.gson.annotations.SerializedName;

public class Block {
    @SerializedName("id")
    private int id;
    @SerializedName("nonce")
    private int nonce;
    @SerializedName("hash")
    private String hash;
    @SerializedName("content")
    private String content;
    @SerializedName("number-of-verifications")
    private int verifNumber;

    public Block(int id, int nonce, String hash, String content, int verifNumber) {
        this.id = id;
        this.nonce = nonce;
        this.hash = hash;
        this.content = content;
        this.verifNumber = verifNumber;
    }

    public int getId() {
        return id;
    }

    public int getNonce() {
        return nonce;
    }

    public String getHash() {
        return hash;
    }

    public String getContent() {
        return content;
    }

    public int getVerifNumber() {
        return verifNumber;
    }
}
