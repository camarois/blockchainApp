package models;

import com.google.gson.annotations.SerializedName;

public class CreateUserRequest {
    @SerializedName("loginRequest")
    private LoginRequest loginRequest;
    @SerializedName("estEditeur")
    private boolean isEditor;
    @SerializedName("estAdmin")
    private boolean isAdmin;

    public CreateUserRequest(LoginRequest loginRequest, boolean isEditor, boolean isAdmin) {
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
