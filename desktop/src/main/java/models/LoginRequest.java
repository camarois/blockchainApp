package models;

import com.google.gson.annotations.SerializedName;

public class LoginRequest {
    @SerializedName("usager")
    private String username;
    @SerializedName("mot_de_passe")
    private String password;

    public LoginRequest(String username, String password) {
        this.username = username;
        this.password = password;
    }

    public String getUsername() {
        return username;
    }

    public String getPassword() {
        return password;
    }
}
