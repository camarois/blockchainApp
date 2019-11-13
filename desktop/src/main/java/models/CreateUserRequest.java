package models;

import com.google.gson.annotations.SerializedName;

public class CreateUserRequest {
    @SerializedName("loginRequest")
    private LoginRequest loginRequest;
    @SerializedName("isEditor")
    private boolean isEditor;
    @SerializedName("isAdmin")
    private boolean isAdmin;

    public CreateUserRequest(LoginRequest loginRequest, boolean isEditor) {
        this.loginRequest = loginRequest;
        this.isEditor = isEditor;
        this.isAdmin = false;
    }

    public LoginRequest getLoginRequest() {
        return loginRequest;
    }

    public boolean getisEditor() {
        return isEditor;
    }

    public boolean getisAdmin() {
        return isAdmin;
    }
}
