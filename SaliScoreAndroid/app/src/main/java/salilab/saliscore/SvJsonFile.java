package salilab.saliscore;

import android.content.Context;

import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.nio.charset.StandardCharsets;

public class SvJsonFile {
  static void write( Context ctx, String data, String fileName ) {
    try {
      OutputStreamWriter outputStreamWriter = new OutputStreamWriter( ctx.openFileOutput(fileName, Context.MODE_PRIVATE) );
      outputStreamWriter.write(data);
      outputStreamWriter.close();
    }
    catch (Exception e) {

    }
  }

  static String readAll( Context ctx, String fileName ) {
    StringBuilder stringBuilder = new StringBuilder();
    try {
      FileInputStream fis = ctx.openFileInput(fileName);
      InputStreamReader inputStreamReader = new InputStreamReader(fis, StandardCharsets.UTF_8);
      BufferedReader reader = new BufferedReader(inputStreamReader);
      String line = reader.readLine();
      while (line != null) {
        stringBuilder.append(line).append('\n');
        line = reader.readLine();
      }
    } catch (Exception e) {
      // Error occurred when opening raw file for reading.
    } finally {
      return stringBuilder.toString();
    }
  }

  static void write( JSONObject obj, Context ctx, String fileName ) {
    write( ctx, obj.toString(), fileName );
  }

  static JSONObject read( Context ctx, String fileName ) {
    try {
      JSONObject obj = new JSONObject(readAll(ctx, fileName));
      return obj;
    }
    catch (Exception e) {
      return new JSONObject();
    }
  }
}
