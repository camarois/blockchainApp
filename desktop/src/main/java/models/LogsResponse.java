package models;

import com.google.gson.annotations.SerializedName;

import java.util.List;
import java.util.Map;

public class LogsResponse {
    @SerializedName("information")
    private List<Log> logs;

    public class Log {
        @SerializedName("no")
        private Integer number;
        @SerializedName("severite")
        private String severity;
        @SerializedName("heure")
        private String hour;
        @SerializedName("message")
        private String message;

        public Log(Integer numberInput, String severityInput, String hourInput, String messageInput) {
            number = numberInput;
            severity = severityInput;
            hour = hourInput;
            message = messageInput;
        }
    }

    public LogsResponse(List<Log> logInfo) {
        logs = logInfo;
    }
}
