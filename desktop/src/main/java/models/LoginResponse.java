package models;

import com.google.gson.annotations.SerializedName;

public class LoginResponse {
    @SerializedName("edition")
    private boolean edition;

    public LoginResponse(boolean edition) {
        this.edition = edition;
    }

    public Boolean getEdition() {
        return edition;
    }
}
