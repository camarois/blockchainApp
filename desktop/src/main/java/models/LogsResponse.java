package models;

import com.google.gson.annotations.SerializedName;

import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Set;

public class LogsResponse {
    @SerializedName("information")
    public Set<Log> logs;

    public class Log {
        @SerializedName("no")
        private Integer number;
        @SerializedName("severite")
        private String severity;
        @SerializedName("heure")
        private String hour;
        private String provenance;
        @SerializedName("message")
        private String message;

        public Log(Integer number, String severity, String hour, String message) {
            this.number = number;
            this.severity = severity;
            this.hour = hour;
            this.message = message;
        }

        void setNumber(int number) {
            this.number = number;
        }

        void setSeverity(String severity) {
            this.severity = severity;
        }

        void setHour(Date hour) {
            this.hour = new SimpleDateFormat("hh:mm:ss").format(hour);
        }

        public void setProvenance(String provenance) {
            this.provenance = provenance;
        }

        void setMessage(String message) {
            this.message = message;
        }

        public Integer getNumber() {
            return number;
        }

        public String getSeverity() {
            return severity;
        }

        public String getHour() {
            return hour;
        }

        public String getProvenance() {
            return provenance;
        }

        public String getMessage() {
            return message;
        }

    }

    public LogsResponse(Set<Log> logInfo) {
        logs = logInfo;
    }

    public Set<Log> getLogs() {
        return logs;
    }
}
