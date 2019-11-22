package models;

import com.google.gson.annotations.SerializedName;

import java.util.ArrayList;


public class AllUsersRequest {
    @SerializedName("utilisateurs")
    private ArrayList<User> users;

    private AllUsersRequest(ArrayList<User> users) {
        this.users = users;
    }

    public ArrayList<User> getUsers() {
        return users;
    }
}
