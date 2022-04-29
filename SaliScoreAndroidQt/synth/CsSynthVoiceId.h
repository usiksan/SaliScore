/*
 Project "SaliScore Score music edit, view and tutorial program"
 Author
   Sibilev Alexander S.
 Web
   SaliLab.com
 Description
   In midi program number and bank both define full voice id. We treate bank number and program number as voiceId
*/
#ifndef CSSYNTHVOICEID_H
#define CSSYNTHVOICEID_H

//!
//! \brief csVoiceIdBank Extract from full voice id bank number
//! \param id            Full voice id
//! \return              Bank number for this voice id
//!
inline int csVoiceIdBank( int id ) { return id >> 7;}

//!
//! \brief csVoiceIdProgra Extract from full voice id program number
//! \param id              Full voice id
//! \return                Program numver for this voice id
//!
inline int csVoiceIdProgram( int id ) { return id & 0x7f; }
inline int csVoiceId( int bank, int program ) { return (bank << 7) | (program & 0x7f); }

#endif // CSSYNTHVOICEID_H
