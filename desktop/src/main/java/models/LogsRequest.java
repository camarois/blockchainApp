package models;

import com.google.gson.annotations.SerializedName;

public class LogsRequest {
    @SerializedName("dernier")
    private Integer last;

    public LogsRequest(Integer last) {
        this.last = last;
    }
}
