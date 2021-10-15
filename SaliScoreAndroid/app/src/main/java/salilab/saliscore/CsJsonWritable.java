package salilab.saliscore;

abstract class CsJsonWritable {
  abstract void jsonWrite( SvJsonWriter js ) throws Exception;

  abstract void jsonRead( SvJsonReader js ) throws Exception;

  abstract CsJsonWritable copy();

}
