#include "score/CsNoteChord.h"
#include "CsSvgBook.h"

CsSvgBook::CsSvgBook() :
  mWidth(0)
  {
  setScale(20);
  }



void CsSvgBook::setScale(int w)
  {
  if( w != mWidth ) {
    //Rebuild
    mWidth = w;

    //Scale matrix
    QMatrix scale;
    double sc = static_cast<double>(w) / 60.0;
    scale.scale( sc, sc );

    //Remove all previous svg's
    qDeleteAll(mSvgList);
    mSvgList.clear();

    QPainterPath a1,a2;



    //2
    a1.clear();
    a2.clear();
    a1.moveTo( -211 , 56 );
    a1.lineTo( -208 , 56 );
    a1.lineTo( -208 , 86 );
    a1.lineTo( -211 , 86 );
    a1.closeSubpath();
    a1.moveTo( -203 , 56 );
    a1.lineTo( -200 , 56 );
    a1.lineTo( -200 , 68.4173 );
    a1.lineTo( -197.425 , 65.357 );
    a1.cubicTo( -190.991 , 57.7105 , -172.294 , 57.3486 , -164.478 , 64.7194 );
    a1.lineTo( -161 , 68 );
    a1.lineTo( -161 , 56 );
    a1.lineTo( -158 , 56 );
    a1.lineTo( -158 , 86 );
    a1.lineTo( -161 , 86 );
    a1.lineTo( -161 , 74.1104 );
    a1.lineTo( -163.575 , 77.1707 );
    a1.cubicTo( -166.463 , 80.6033 , -173.854 , 83.2638 , -180.5 , 83.2638 );
    a1.cubicTo( -187.146 , 83.2638 , -194.537 , 80.6033 , -197.425 , 77.1707 );
    a1.lineTo( -200 , 74.1104 );
    a1.lineTo( -200 , 86 );
    a1.lineTo( -203 , 86 );
    a1.closeSubpath();
    a1.moveTo( -173.398 , 78.7481 );
    a1.cubicTo( -170.339 , 74.0802 , -173.263 , 64.8317 , -178.561 , 62.418 );
    a1.cubicTo( -186.341 , 58.8733 , -191.11 , 64.9138 , -187.869 , 74.2088 );
    a1.cubicTo( -185.628 , 80.6379 , -176.51 , 83.498 , -173.398 , 78.7481 );
    a1.closeSubpath();
    a1.moveTo( -153 , 56 );
    a1.lineTo( -150 , 56 );
    a1.lineTo( -150 , 86 );
    a1.lineTo( -153 , 86 );
    a1.closeSubpath();

    //up
    mSvgList.append( new CsSvg( scale.map(a2), scale.map(a1.toFillPolygon( QTransform(0.81549302,0,0,0.81549302,172.25963,89.950654))), false ) );
    //dn
    mSvgList.append( new CsSvg( scale.map(a2), scale.map(a1.toFillPolygon( QTransform(0.81549302,0,0,0.81549302,171.96406,-45.601725))), false ) );



    //1
    a1.clear();
    a2.clear();
    a1.moveTo( 225.363 , 99.818 );
    a1.cubicTo( 219.866 , 98.1261 , 215.529 , 93.5286 , 215.529 , 89.3934 );
    a1.cubicTo( 215.529 , 77.6892 , 240.668 , 73.0575 , 251.05 , 82.849 );
    a1.cubicTo( 262.277 , 93.4372 , 243.955 , 105.54 , 225.363 , 99.818 );
    a1.closeSubpath();
    a1.moveTo( 242.131 , 96.8777 );
    a1.cubicTo( 245.19 , 92.2098 , 242.265 , 82.9613 , 236.968 , 80.5475 );
    a1.cubicTo( 229.188 , 77.0028 , 224.419 , 83.0434 , 227.659 , 92.3383 );
    a1.cubicTo( 229.901 , 98.7674 , 239.019 , 101.628 , 242.131 , 96.8777 );
    a1.closeSubpath();

    //up
    mSvgList.append( new CsSvg( scale.map(a2), scale.map(a1.toFillPolygon( QTransform(0.9996359,0,0,0.9996359,-215.3115,58.796482))), false ) );
    //dn
    mSvgList.append( new CsSvg( scale.map(a2), scale.map(a1.toFillPolygon( QTransform(0.9996359,0,0,0.9996359,-215.17749,-77.263165))), false ) );



    //1/2
    a1.clear();
    a2.clear();
    a1.moveTo( 85.0937 , 139.349 );
    a1.cubicTo( 85.0937 , 134.825 , 87.2435 , 130.822 , 91.5432 , 127.342 );
    a1.cubicTo( 95.8428 , 123.774 , 100.395 , 121.991 , 105.201 , 121.991 );
    a1.cubicTo( 107.983 , 121.991 , 110.47 , 122.73 , 112.662 , 124.209 );
    a1.lineTo( 112.662 , 46.9428 );
    a1.lineTo( 115.444 , 46.9428 );
    a1.lineTo( 115.444 , 131.388 );
    a1.cubicTo( 115.444 , 136.261 , 113.379 , 140.307 , 109.248 , 143.526 );
    a1.cubicTo( 105.117 , 146.745 , 100.564 , 148.355 , 95.5899 , 148.355 );
    a1.cubicTo( 92.8078 , 148.355 , 90.3629 , 147.529 , 88.2552 , 145.875 );
    a1.cubicTo( 86.1475 , 144.135 , 85.0937 , 141.96 , 85.0937 , 139.349 );
    a1.moveTo( 90.9109 , 143.265 );
    a1.cubicTo( 95.3792 , 143.265 , 99.8896 , 141.307 , 104.442 , 137.392 );
    a1.cubicTo( 109.079 , 133.476 , 111.397 , 130.561 , 111.398 , 128.647 );
    a1.cubicTo( 111.397 , 127.429 , 110.892 , 126.82 , 109.88 , 126.82 );
    a1.cubicTo( 106.761 , 126.82 , 102.461 , 128.69 , 96.981 , 132.432 );
    a1.cubicTo( 91.5853 , 136.087 , 88.8875 , 139.132 , 88.8875 , 141.568 );
    a1.cubicTo( 88.8875 , 142.699 , 89.562 , 143.265 , 90.9109 , 143.265 );

    //up
    mSvgList.append( new CsSvg( scale.map(a2), scale.map(a1.toFillPolygon( QTransform(0.96160158,0,0,0.95078597,-81.588417,19.086899))), false ) );
    //dn
    mSvgList.append( new CsSvg( scale.map(a2), scale.map(a1.toFillPolygon( QTransform(-0.96160158,0,0,-0.95078597,111.31443,141.3141))), false ) );


    //1/4 up
    a1.clear();
    a2.clear();
    a1.moveTo( 153.038 , 108.771 );
    a1.cubicTo( 153.038 , 112.641 , 148.414 , 115.844 , 142.484 , 116.079 );
    a1.cubicTo( 136.554 , 116.315 , 131.367 , 113.503 , 130.646 , 109.66 );
    a1.cubicTo( 129.924 , 105.818 , 133.916 , 102.274 , 139.758 , 101.57 );
    a1.cubicTo( 145.6 , 100.867 , 151.273 , 103.248 , 152.706 , 107.004 );
    a1 = QMatrix(1.2477873,-0.63522684,0.59622185,1.3694102,-142.33379,929.29558).map(a1);

    QTransform t1;
    t1.translate(-84.221634,-840.67061);

    a2.moveTo(113.13433,900.47269);
    a2.lineTo(113.13433,983.61288);

    //up
    mSvgList.append( new CsSvg( scale.map(t1.map(a2)), scale.map(a1.toFillPolygon( t1 )), false ) );
    //dn
    t1 = QTransform();
    t1.translate( 57.297576, 500.33615 );
    t1.rotate( 180 );
    t1.translate( -57.297576, -500.33615 );
    mSvgList.append( new CsSvg( scale.map(t1.map(a2)), scale.map(a1.toFillPolygon( t1 )), false ) );



    //1/8 up
    a1.clear();
    a2.clear();
    a1.moveTo( 250.303 , 43.3633 );
    a1.lineTo( 250.303 , 117.895 );
    a1.cubicTo( 248.108 , 114.762 , 239.185 , 114.29 , 232.928 , 117.738 );
    a1.cubicTo( 225.512 , 121.825 , 222.13 , 129.489 , 225.365 , 134.863 );
    a1.cubicTo( 228.6 , 140.237 , 237.231 , 141.294 , 244.647 , 137.207 );
    a1.cubicTo( 249.098 , 134.754 , 252.065 , 131.002 , 253.053 , 127.238 );
    a1.cubicTo( 253.505 , 125.169 , 253.241 , 124.096 , 253.209 , 122.645 );
    a1.lineTo( 253.209 , 69.9883 );
    a1.cubicTo( 285.743 , 78.2298 , 270.728 , 106.799 , 267.802 , 116.891 );
    a1.cubicTo( 294.53 , 80.3853 , 253.574 , 65.4951 , 253.209 , 43.3633 );
    a1.closeSubpath();

    //up
    mSvgList.append( new CsSvg( scale.map(a2), scale.map(a1.toFillPolygon( QTransform().translate(-223.9611,20.270172) )), false ) );



    //1/8 dn
    a1.clear();
    a2.clear();

    //dn
    mSvgList.append( new CsSvg( scale.map(a2), scale.map(a1.toFillPolygon( QTransform().translate(-223.9611,20.270172) )), false ) );




    //1/16 up
    a1.clear();
    a2.clear();
    a1.moveTo( 110.938 , 51.9375 );
    a1.lineTo( 110.938 , 126 );
    a1.cubicTo( 109.063 , 123.457 , 101.179 , 121.802 , 94.375 , 126.156 );
    a1.cubicTo( 87.5711 , 130.51 , 83.4865 , 137.791 , 85.8438 , 142.094 );
    a1.cubicTo( 88.5345 , 147.006 , 97.4794 , 149.635 , 104.594 , 144.531 );
    a1.cubicTo( 109.739 , 140.841 , 112.212 , 136.098 , 113 , 134.688 );
    a1.lineTo( 113 , 90.0313 );
    a1.cubicTo( 132.08 , 101.689 , 142.431 , 105.023 , 132.813 , 131.344 );
    a1.cubicTo( 151.189 , 103.266 , 128.275 , 99.5763 , 113 , 72.0313 );
    a1.lineTo( 113 , 69.9375 );
    a1.cubicTo( 138.867 , 83.0557 , 133.677 , 95.3289 , 136 , 102.063 );
    a1.cubicTo( 142.435 , 75.1018 , 119.34 , 72.3758 , 113 , 53.25 );
    a1.lineTo( 113 , 52.0938 );
    a1.closeSubpath();

    mSvgList.append( new CsSvg( scale.map(a2), scale.map(a1.toFillPolygon( QTransform(1.0442149,0,0,1.0196409,-88.525037,9.7839528) )), false ) );

    //1/16 dn
    a1.clear();
    a2.clear();

    mSvgList.append( new CsSvg( scale.map(a2), scale.map(a1.toFillPolygon( QTransform(1.0442149,0,0,1.0196409,-88.525037,9.7839528) )), false ) );



    //1/32 up
    a1.clear();
    a2.clear();
    a1.moveTo( 111.875 , 37.9063 );
    a1.lineTo( 112.063 , 125.469 );
    a1.cubicTo( 110.188 , 122.926 , 102.304 , 121.271 , 95.5 , 125.625 );
    a1.cubicTo( 88.6961 , 129.979 , 84.6115 , 137.259 , 86.9688 , 141.563 );
    a1.cubicTo( 89.6595 , 146.474 , 98.6044 , 149.104 , 105.719 , 144 );
    a1.cubicTo( 110.864 , 140.309 , 113.337 , 135.567 , 114.125 , 134.156 );
    a1.lineTo( 114.125 , 89.5 );
    a1.cubicTo( 133.017 , 101.97 , 142.306 , 104.366 , 132.688 , 130.688 );
    a1.cubicTo( 142.13 , 118.492 , 141.449 , 110.622 , 136.5 , 102.469 );
    a1.cubicTo( 137.239 , 96.9836 , 137.277 , 92.7804 , 136.781 , 89.4063 );
    a1.cubicTo( 142.191 , 58.1134 , 123.114 , 60.8015 , 114.125 , 41.125 );
    a1.lineTo( 114.125 , 38.0625 );
    a1.closeSubpath();
    a1.moveTo( 114.125 , 56.125 );
    a1.cubicTo( 133.101 , 66.3641 , 134.846 , 76.3983 , 135.344 , 84.125 );
    a1.cubicTo( 131.667 , 75.5781 , 122.838 , 72.9732 , 114.125 , 58.7813 );
    a1.closeSubpath();
    a1.moveTo( 114.125 , 71.5 );
    a1.cubicTo( 133.357 , 87.8678 , 133.283 , 84.0042 , 135.594 , 101.031 );
    a1.cubicTo( 130.343 , 93.1019 , 121.414 , 84.6442 , 114.125 , 71.5 );
    a1.closeSubpath();

    mSvgList.append( new CsSvg( scale.map(a2), scale.map(a1.toFillPolygon( QTransform(1.072597,0,0,1.0404139,-92.512146,7.0542639))), false ) );


    //1/32 dn
    a1.clear();
    a2.clear();

    mSvgList.append( new CsSvg( scale.map(a2), scale.map(a1.toFillPolygon( QTransform(1.072597,0,0,1.0404139,-92.512146,7.0542639))), false ) );



    //1/64 up
    a1.clear();
    a2.clear();
    a1.moveTo( 109.506 , 229.551 );
    a1.lineTo( 109.506 , 351.426 );
    a1.cubicTo( 106.737 , 348.808 , 98.9167 , 348.995 , 92.8887 , 352.361 );
    a1.cubicTo( 86.8607 , 355.726 , 83.1291 , 360.789 , 83.2765 , 366.383 );
    a1.cubicTo( 83.3954 , 370.896 , 88.3269 , 374.235 , 93.2145 , 374.235 );
    a1.cubicTo( 98.1021 , 374.235 , 107.551 , 372.038 , 111.461 , 360.26 );
    a1.cubicTo( 111.624 , 344.181 , 111.461 , 310.856 , 111.461 , 310.856 );
    a1.cubicTo( 119.119 , 317.96 , 126.811 , 320.773 , 129.871 , 325.813 );
    a1.cubicTo( 135.909 , 335.754 , 130.034 , 352.922 , 129.383 , 355.539 );
    a1.cubicTo( 131.137 , 352.95 , 134.644 , 345.898 , 134.586 , 337.479 );
    a1.cubicTo( 134.552 , 332.432 , 134.596 , 322.073 , 124.495 , 312.165 );
    a1.cubicTo( 114.394 , 302.256 , 111.404 , 297.295 , 111.404 , 287.947 );
    a1.cubicTo( 118.85 , 295.099 , 133.199 , 299.926 , 131.596 , 322.141 );
    a1.cubicTo( 132.141 , 323.458 , 132.524 , 324.6 , 132.524 , 324.6 );
    a1.cubicTo( 134.626 , 316.952 , 134.107 , 305.06 , 129.22 , 297.582 );
    a1.cubicTo( 124.332 , 290.104 , 118.286 , 286.757 , 114.72 , 281.13 );
    a1.cubicTo( 112.113 , 277.017 , 111.567 , 274.828 , 111.404 , 268.471 );
    a1.cubicTo( 118.735 , 274.08 , 126.785 , 280.727 , 128.568 , 284.682 );
    a1.cubicTo( 132.499 , 293.401 , 132.2 , 299.707 , 131.874 , 303.072 );
    a1.cubicTo( 132.832 , 305.644 , 132.639 , 306.314 , 132.639 , 306.314 );
    a1.cubicTo( 132.639 , 306.314 , 133.646 , 302.496 , 133.646 , 297.261 );
    a1.cubicTo( 133.646 , 292.026 , 133.781 , 286.925 , 129.057 , 278.138 );
    a1.cubicTo( 124.332 , 269.351 , 115.713 , 265.145 , 113.579 , 260.003 );
    a1.cubicTo( 111.95 , 256.077 , 111.543 , 254.768 , 111.461 , 249.534 );
    a1.cubicTo( 118.956 , 254.768 , 122.703 , 257.012 , 126.45 , 262.995 );
    a1.cubicTo( 130.197 , 268.977 , 131.914 , 270.633 , 131.769 , 283.892 );
    a1.cubicTo( 132.583 , 285.949 , 132.478 , 286.551 , 132.478 , 286.551 );
    a1.cubicTo( 132.478 , 286.551 , 133.925 , 278.786 , 133.493 , 272.56 );
    a1.cubicTo( 133.13 , 267.311 , 131.338 , 261.686 , 127.102 , 255.89 );
    a1.cubicTo( 122.866 , 250.095 , 116.801 , 244.315 , 113.742 , 240.373 );
    a1.cubicTo( 111.72 , 237.766 , 111.54 , 236.268 , 111.519 , 234.448 );
    a1.lineTo( 111.462 , 229.529 );
    a1.closeSubpath();

    mSvgList.append( new CsSvg( scale.map(a2), scale.map(a1.toFillPolygon( QTransform().translate(-82.84375,-214.4375) )), false ) );


    //1/64 up
    a1.clear();
    a2.clear();

    mSvgList.append( new CsSvg( scale.map(a2), scale.map(a1.toFillPolygon( QTransform().translate(-82.84375,-214.4375) )), false ) );


    //1/128 up
    a1.clear();
    a2.clear();
    a1.moveTo( 29.238 , -32.553 );
    a1.cubicTo( 29.3213 , -27.4452 , 29.7959 , -26.1681 , 31.4592 , -22.3374 );
    a1.cubicTo( 33.6373 , -17.3199 , 42.4368 , -13.2161 , 47.26 , -4.64243 );
    a1.cubicTo( 52.0834 , 3.93128 , 51.9458 , 8.90872 , 51.9458 , 14.0162 );
    a1.cubicTo( 51.9458 , 19.1244 , 50.9173 , 22.8499 , 50.9173 , 22.8499 );
    a1.cubicTo( 50.9173 , 22.8499 , 51.1142 , 22.1958 , 50.1362 , 19.6864 );
    a1.cubicTo( 50.4691 , 16.4025 , 50.7741 , 10.2501 , 46.7611 , 1.74217 );
    a1.cubicTo( 44.9409 , -2.11741 , 36.6637 , -8.60277 , 29.1787 , -14.0754 );
    a1.cubicTo( 29.262 , -8.96757 , 29.7366 , -7.69043 , 31.3999 , -3.85975 );
    a1.cubicTo( 33.578 , 1.15777 , 42.3776 , 5.26151 , 47.2007 , 13.8352 );
    a1.cubicTo( 52.0241 , 22.4089 , 51.8865 , 27.3864 , 51.8865 , 32.4938 );
    a1.cubicTo( 51.8865 , 37.602 , 50.858 , 41.3276 , 50.858 , 41.3276 );
    a1.cubicTo( 50.858 , 41.3276 , 51.055 , 40.6735 , 50.0769 , 38.164 );
    a1.cubicTo( 50.4098 , 34.8801 , 50.7148 , 28.7278 , 46.7018 , 20.2198 );
    a1.cubicTo( 44.8817 , 16.3602 , 36.6633 , 9.87488 , 29.1784 , 4.40226 );
    a1.cubicTo( 29.3447 , 10.6048 , 29.9025 , 12.7405 , 32.5637 , 16.7536 );
    a1.cubicTo( 36.2046 , 22.2442 , 42.3772 , 25.51 , 47.3667 , 32.807 );
    a1.cubicTo( 52.3564 , 40.1035 , 52.8859 , 51.7072 , 50.7405 , 59.1696 );
    a1.cubicTo( 50.7405 , 59.1696 , 50.3489 , 58.055 , 49.7927 , 56.7706 );
    a1.cubicTo( 51.4294 , 35.0932 , 36.7806 , 30.3834 , 29.1781 , 23.4051 );
    a1.cubicTo( 29.1781 , 32.5263 , 32.2311 , 37.3672 , 42.5432 , 47.0357 );
    a1.cubicTo( 52.8553 , 56.7045 , 52.8102 , 66.8114 , 52.8454 , 71.7368 );
    a1.cubicTo( 52.9044 , 79.9515 , 49.3241 , 86.8317 , 47.533 , 89.3581 );
    a1.cubicTo( 48.1982 , 86.8042 , 54.1956 , 70.0534 , 48.0319 , 60.3528 );
    a1.cubicTo( 44.9074 , 55.4349 , 37.0542 , 52.6907 , 29.237 , 45.7589 );
    a1.cubicTo( 29.237 , 45.7589 , 29.4033 , 78.2757 , 29.237 , 93.9643 );
    a1.cubicTo( 25.2451 , 105.457 , 15.5982 , 107.601 , 10.6084 , 107.601 );
    a1.cubicTo( 5.61869 , 107.601 , 0.583815 , 104.342 , 0.462301 , 99.9389 );
    a1.cubicTo( 0.311802 , 94.4804 , 4.1213 , 89.5405 , 10.2758 , 86.257 );
    a1.cubicTo( 16.43 , 82.9732 , 24.4136 , 82.7908 , 27.2414 , 85.3451 );
    a1.lineTo( 27.2316 , -52.0521 );
    a1.lineTo( 29.2374 , -52.0738 );
    a1.lineTo( 29.2964 , -47.2744 );
    a1.cubicTo( 29.3178 , -45.4981 , 29.5012 , -44.0368 , 31.5663 , -41.493 );
    a1.cubicTo( 34.6885 , -37.6468 , 40.8809 , -32.007 , 45.2052 , -26.3519 );
    a1.cubicTo( 49.53 , -20.6969 , 51.3597 , -15.2077 , 51.7308 , -10.0869 );
    a1.cubicTo( 52.1708 , -4.01146 , 50.6941 , 3.56575 , 50.6941 , 3.56575 );
    a1.cubicTo( 50.6941 , 3.56575 , 50.8015 , 2.97811 , 49.97 , 0.971028 );
    a1.cubicTo( 50.1178 , -11.9665 , 48.3659 , -13.5824 , 44.5403 , -19.4198 );
    a1.cubicTo( 40.7146 , -25.2572 , 36.8893 , -27.4456 , 29.2381 , -32.5531 );

    mSvgList.append( new CsSvg( scale.map(a2), scale.map(a1.toFillPolygon( QTransform() )), false ) );



    //1/128 up
    a1.clear();
    a2.clear();

    mSvgList.append( new CsSvg( scale.map(a2), scale.map(a1.toFillPolygon( QTransform() ) ), false ) );


    }
  }



CsSvg *CsSvgBook::svgNote(int duration, bool up) const
  {
  if( up ) {
    switch( duration ) {
      case duraBreve : return mSvgList.at(0);
      case duraHole : return mSvgList.at(2);
      case duraHalf : return mSvgList.at(4);
      case duraQuarter : return mSvgList.at(6);
      case duraEighth : return mSvgList.at(8);
      case duraSixteenth : return mSvgList.at(10);
      case duraThirtySecond : return mSvgList.at(12);
      case duraSixtyFourth : return mSvgList.at(14);
      case duraOneHundredTwentyEighth : return mSvgList.at(16);
      }
    }
  else {
    switch( duration ) {
      case duraBreve : return mSvgList.at(1);
      case duraHole : return mSvgList.at(3);
      case duraHalf : return mSvgList.at(5);
      case duraQuarter : return mSvgList.at(7);
      case duraEighth : return mSvgList.at(9);
      case duraSixteenth : return mSvgList.at(11);
      case duraThirtySecond : return mSvgList.at(13);
      case duraSixtyFourth : return mSvgList.at(15);
      case duraOneHundredTwentyEighth : return mSvgList.at(17);
      }
    }
  return nullptr;
  }
