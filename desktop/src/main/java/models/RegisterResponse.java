package models;

import com.google.gson.annotations.SerializedName;
import com.google.gson.internal.$Gson$Preconditions;

public class RegisterResponse {
    @SerializedName("edition")
    private boolean edition;

    public RegisterResponse(boolean edition) {
        this.edition = edition;
    }

    public boolean getEdition() {
        return edition;
    }
}
