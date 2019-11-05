package models;

import com.google.gson.annotations.SerializedName;

import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.List;

public class LogsResponse {
    @SerializedName("information")
    public List<Log> logs;

    public static class Log {
        enum Severity {
            ERROR(0), ATTENTION(1), INFO(2);

            private final int value;
            Severity(int value) {
                this.value = value;
            }
        }

        @SerializedName("no")
        private Integer number;
        @SerializedName("severite")
        private Integer severity;
        @SerializedName("heure")
        private String hour;
        private String provenance;
        @SerializedName("message")
        private String message;

        public Log(Integer number, Integer severity, String hour, String message) {
            this.number = number;
            this.severity = severity;
            this.hour = hour;
            this.message = message;
        }

        void setNumber(int number) {
            this.number = number;
        }

        void setSeverity(int severity) {
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
            return Severity.values()[severity].name();
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

    public LogsResponse(List<Log> logInfo) {
        logs = logInfo;
    }
}
