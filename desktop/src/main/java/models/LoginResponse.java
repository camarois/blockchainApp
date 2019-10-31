package models;

import com.google.gson.annotations.SerializedName;

public class LoginResponse {
    @SerializedName("edition")
    private boolean edition;

    public LoginResponse(boolean admin) {
        edition = admin;
    }

    public Boolean getEdition() {
        return edition;
    }
}
