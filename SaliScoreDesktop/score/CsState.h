#ifndef CSSTATE_H
#define CSSTATE_H




class CsState
  {
  public:
    CsState();


    void jsonWrite( CsJsonWriter &js ) const;

    void jsonRead( CsJsonReader &js );

  };

#endif // CSSTATE_H
