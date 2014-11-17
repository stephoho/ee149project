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

    public BlockNotification() {
        content = "";
        appName = "";
        time = new DateTime();
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


}
