package models;

import com.google.gson.annotations.SerializedName;

public class ChainResponse {
    @SerializedName("bloc")
    private String bloc;
    @SerializedName("suivant")
    private ChainResponse nextBloc;

    public ChainResponse(String bloc, ChainResponse nextBloc) {
        this.bloc = bloc;
        this.nextBloc = nextBloc;
    }
}
