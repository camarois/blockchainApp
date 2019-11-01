package models;

import com.google.gson.annotations.SerializedName;

public class ChaineRequest {
    @SerializedName("dernier_blocs")
    private Integer lastBlocs;

    public ChaineRequest(Integer lastBlocsInput) {
        lastBlocs = lastBlocsInput;
    }
}
