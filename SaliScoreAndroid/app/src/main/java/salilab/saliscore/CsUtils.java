package salilab.saliscore;

public class CsUtils {
  static int qBound( int min, int val, int max ) {
    if( val < min ) return min;
    if( val > max ) return max;
    return val;
  }
}
