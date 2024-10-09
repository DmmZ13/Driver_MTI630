#define message_default "FA FF 36 4E 10 60 04 02 53 35 CB 20 10 10 3F 7F FA 66 BB 40 34 B8 3C 49 96 2D 3B 5A 1F 82 80 20 0C 39 FF 91 01 BA EC DD 81 BA 86 53 80 40 20 0C BE 64 DB 99 BC E7 FF 83 41 1D C2 CF C0 20 0C BE 90 48 80 3F 67 32 04 BF 54 89 04 E0 20 04 00 00 00 03 C9"

#define y "0"

/* defines y indepedent*/

    /* define Timestamp */

    #define XDI_UtcTime "1010"
    #define XDI_PacketCounter "1020"
    #define XDI_SampleTimeFine "1060"
    #define XDI_SampleTimeCoarse "1070"

    /* define Pressure */

    #define XDI_BaroPressure "3010"

    /* define Status */

    #define XDI_StatusByte "E010"
    #define XDI_StatusWord "E020"

/* defines y depedent*/  

    /* define Temperature */

    #define XDI_Temperature "081"

    /* define Orientation Data */

    #define XDI_Quaternion "201"
    #define XDI_RotationMatrix "202"
    #define XDI_EulerAngles "203"



    /* define Acceleration */

    #define XDI_DeltaV "401"
    #define XDI_Acceleration "402"
    #define XDI_FreeAcceleration "403"
    #define XDI_AccelerationHR "404"

    /* define Angular Velocity */

    #define XDI_RateOfTurn "802"
    #define XDI_DeltaQ "803"
    #define	XDI_RateOfTurnHR "804"

    /* define MagneticGroup */

    #define XDI_MAGNETICFIELD = "C02"

/* define functions */

void une_msg(char string[]){
} 