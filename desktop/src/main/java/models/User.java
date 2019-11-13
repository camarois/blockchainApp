package models;

import com.google.gson.annotations.SerializedName;

public class User {
    @SerializedName("loginRequest")
    private LoginRequest loginRequest;
    @SerializedName("isEditor")
    private boolean isEditor;
    @SerializedName("isAdmin")
    private boolean isAdmin;

    public User(LoginRequest loginRequest, boolean isEditor, boolean isAdmin) {
        this.loginRequest = loginRequest;
        this.isEditor = isEditor;
        this.isAdmin = isAdmin;
    }

    public LoginRequest getLoginRequest() {
        return loginRequest;
    }

    public boolean getIsEditor() {
        return isEditor;
    }

    public boolean getIsAdmin() {
        return isAdmin;
    }
}
