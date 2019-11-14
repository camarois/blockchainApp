package models;

import com.google.gson.annotations.SerializedName;

public class User {
    @SerializedName("usager")
    private String username;
    @SerializedName("estEditeur")
    private boolean isEditor;
    @SerializedName("estAdmin")
    private boolean isAdmin;

    public User(String username, boolean isEditor, boolean isAdmin) {
        this.username = username;
        this.isEditor = isEditor;
        this.isAdmin = isAdmin;
    }

    public String getUsername() {
        return username;
    }

    public String getIsEditor() {
        return isEditor ? "Oui" : "";
    }

    public String getIsAdmin() {
        return isAdmin ? "Oui" : "";
    }

}
