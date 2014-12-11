package com.eecs149.block;

import org.joda.time.DateTime;
import org.joda.time.format.DateTimeFormat;
import org.joda.time.format.DateTimeFormatter;

/**
 * Created by stephanieho on 11/16/14.
 */
public class BlockNotification {

    private DateTimeFormatter format = DateTimeFormat.forPattern("hh:mma");

    private String content;
    private String appName;
    private DateTime time;

    public BlockNotification(String app, String cntnt, long timePosted) {
        content = cntnt;
        appName = app;
        time = new DateTime(timePosted);
    }

    public String getContent() {
        return content;
    }

    public String getAppName() {
        return appName;
    }

    public DateTime getDateTime() {
        return time;
    }

    public String getTimgeString() {
        return format.print(time);
    }


    // map app to color
    // TODO: map to ints instead
    public String getColor() {
        if (appName.contains("facebook")){
            return "blue";
        } else if (appName.contains("google") && appName.contains("talk")) {
            return "green";
        } else if (appName.contains("google") && appName.contains("gm")) {
            return "red";
        } else if (appName.contains("naver") && appName.contains("line")) {
            return "white";
        } else {
            return "";
        }
    }


}
