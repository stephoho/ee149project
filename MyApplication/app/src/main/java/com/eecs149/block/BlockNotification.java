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

    public String getTimeString() {
        return format.print(time);
    }


    // map app package to short name to send
    public String getShortName() {
        return getShortName(this.appName);
    }

    public static String getShortName(String input) {
        if (input.contains("facebook")) {
            return "facebook";
        } else if (input.contains("com.facebook.orca")) {
            return "fbmessenger";
        } else if (input.contains("com.google.android.gm")) {
            return "gmail";
        } else if (input.contains("com.google.android.talk")) {
            return "hangouts";
        } else if (input.contains("jp.naver.line.android")) {
            return "line";
        } else if (input.contains("com.snapchat.android")) {
            return "snapchat";
        } else {
            return "other";
        }

    }


}
