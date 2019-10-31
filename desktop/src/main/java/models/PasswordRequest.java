package models;

import com.google.gson.annotations.SerializedName;

public class PasswordRequest {
    @SerializedName("ancien")
    private String oldPassword;
    @SerializedName("nouveau")
    private String newPassword;

    public PasswordRequest(String oldPasswordInput, String newPasswordInput) {
        oldPassword = oldPasswordInput;
        newPassword = newPasswordInput;
    }
}
