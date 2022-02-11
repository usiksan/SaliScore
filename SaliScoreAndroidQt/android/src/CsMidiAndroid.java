
import android.content.Context;
import android.content.pm.PackageManager;

import android.media.midi.MidiDeviceInfo;
import android.media.midi.MidiManager;


public class CsMidiAndroid {
  public int id;
  public CsMidiAndroid( Context ctx )
    {
    this.id = 5;
    System.out.println("Created SomeJavaClass object with id: " + this.id);
    if( ctx.getPackageManager().hasSystemFeature(PackageManager.FEATURE_MIDI) ) {
      // do MIDI stuff
      MidiManager midiManager = (MidiManager) ctx.getSystemService(Context.MIDI_SERVICE);
      this.id = 6;
      }
    }

  public String sayHello()
    {
    return "SomeJavaClass object number: " + id + " say hello :)";
    }


  public static double multiply(double a, double b)
    {
    return  a * b;
    }
}
