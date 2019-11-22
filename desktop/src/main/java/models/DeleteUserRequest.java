package models;

import com.google.gson.annotations.SerializedName;

public class DeleteUserRequest {
    @SerializedName("usager")
    String username;

    public DeleteUserRequest(String username) {
        this.username = username;
    }

    public String getUsername() {
        return username;
    }
}
