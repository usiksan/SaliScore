
#include "qplatformdefs.h"

#include "qpen.h"
#include "qpainterpath.h"
#include "qbrush.h"
#include "qcolor.h"
#include "qtextformat.h"
#include "qvector.h"
#include "qfileinfo.h"
#include "qfile.h"
#include "qdir.h"
#include "qdebug.h"
#include "qmath.h"
#include "qnumeric.h"
#include "qvarlengtharray.h"
//#include "private/qmath_p.h"

#include "float.h"

#include <QPainter>

static const char *qt_inherit_text = "inherit";

#define Q_PI 3.14156


// ======== duplicated from qcolor_p

static inline int qsvg_h2i(char hex)
{
    if (hex >= '0' && hex <= '9')
        return hex - '0';
    if (hex >= 'a' && hex <= 'f')
        return hex - 'a' + 10;
    if (hex >= 'A' && hex <= 'F')
        return hex - 'A' + 10;
    return -1;
}

static inline int qsvg_hex2int(const char *s)
{
    return (qsvg_h2i(s[0]) << 4) | qsvg_h2i(s[1]);
}

static inline int qsvg_hex2int(char s)
{
    int h = qsvg_h2i(s);
    return (h << 4) | h;
}

bool qsvg_get_hex_rgb(const char *name, QRgb *rgb)
{
    if(name[0] != '#')
        return false;
    name++;
    int len = qstrlen(name);
    int r, g, b;
    if (len == 12) {
        r = qsvg_hex2int(name);
        g = qsvg_hex2int(name + 4);
        b = qsvg_hex2int(name + 8);
    } else if (len == 9) {
        r = qsvg_hex2int(name);
        g = qsvg_hex2int(name + 3);
        b = qsvg_hex2int(name + 6);
    } else if (len == 6) {
        r = qsvg_hex2int(name);
        g = qsvg_hex2int(name + 2);
        b = qsvg_hex2int(name + 4);
    } else if (len == 3) {
        r = qsvg_hex2int(name[0]);
        g = qsvg_hex2int(name[1]);
        b = qsvg_hex2int(name[2]);
    } else {
        r = g = b = -1;
    }
    if ((uint)r > 255 || (uint)g > 255 || (uint)b > 255) {
        *rgb = 0;
        return false;
    }
    *rgb = qRgb(r, g ,b);
    return true;
}

bool qsvg_get_hex_rgb(const QChar *str, int len, QRgb *rgb)
{
    if (len > 13)
        return false;
    char tmp[16];
    for(int i = 0; i < len; ++i)
        tmp[i] = str[i].toLatin1();
    tmp[len] = 0;
    return qsvg_get_hex_rgb(tmp, rgb);
}

// ======== end of qcolor_p duplicate

static bool parsePathDataFast(const QStringRef &data, QPainterPath &path);



static const char * QSvgStyleSelector_nodeString[] = {
    "svg",
    "g",
    "defs",
    "switch",
    "animation",
    "arc",
    "circle",
    "ellipse",
    "image",
    "line",
    "path",
    "polygon",
    "polyline",
    "rect",
    "text",
    "textarea",
    "tspan",
    "use",
    "video"
};


// '0' is 0x30 and '9' is 0x39
static inline bool isDigit(ushort ch)
{
    static quint16 magic = 0x3ff;
    return ((ch >> 4) == 3) && (magic >> (ch & 15));
}

static qreal toDouble(const QChar *&str)
{
    const int maxLen = 255;//technically doubles can go til 308+ but whatever
    char temp[maxLen+1];
    int pos = 0;

    if (*str == QLatin1Char('-')) {
        temp[pos++] = '-';
        ++str;
    } else if (*str == QLatin1Char('+')) {
        ++str;
    }
    while (isDigit(str->unicode()) && pos < maxLen) {
        temp[pos++] = str->toLatin1();
        ++str;
    }
    if (*str == QLatin1Char('.') && pos < maxLen) {
        temp[pos++] = '.';
        ++str;
    }
    while (isDigit(str->unicode()) && pos < maxLen) {
        temp[pos++] = str->toLatin1();
        ++str;
    }
    bool exponent = false;
    if ((*str == QLatin1Char('e') || *str == QLatin1Char('E')) && pos < maxLen) {
        exponent = true;
        temp[pos++] = 'e';
        ++str;
        if ((*str == QLatin1Char('-') || *str == QLatin1Char('+')) && pos < maxLen) {
            temp[pos++] = str->toLatin1();
            ++str;
        }
        while (isDigit(str->unicode()) && pos < maxLen) {
            temp[pos++] = str->toLatin1();
            ++str;
        }
    }

    temp[pos] = '\0';

    qreal val;
    if (!exponent && pos < 10) {
        int ival = 0;
        const char *t = temp;
        bool neg = false;
        if(*t == '-') {
            neg = true;
            ++t;
        }
        while(*t && *t != '.') {
            ival *= 10;
            ival += (*t) - '0';
            ++t;
        }
        if(*t == '.') {
            ++t;
            int div = 1;
            while(*t) {
                ival *= 10;
                ival += (*t) - '0';
                div *= 10;
                ++t;
            }
            val = ((qreal)ival)/((qreal)div);
        } else {
            val = ival;
        }
        if (neg)
            val = -val;
    } else {
        val = QByteArray::fromRawData(temp, pos).toDouble();
    }
    return val;

}
static qreal toDouble(const QString &str, bool *ok = NULL)
{
    const QChar *c = str.constData();
    qreal res = toDouble(c);
    if (ok) {
        *ok = ((*c) == QLatin1Char('\0'));
    }
    return res;
}

static qreal toDouble(const QStringRef &str, bool *ok = NULL)
{
    const QChar *c = str.constData();
    qreal res = toDouble(c);
    if (ok) {
        *ok = (c == (str.constData() + str.length()));
    }
    return res;
}

static QVector<qreal> parseNumbersList(const QChar *&str)
{
    QVector<qreal> points;
    if (!str)
        return points;
    points.reserve(32);

    while (str->isSpace())
        ++str;
    while (isDigit(str->unicode()) ||
           *str == QLatin1Char('-') || *str == QLatin1Char('+') ||
           *str == QLatin1Char('.')) {

        points.append(toDouble(str));

        while (str->isSpace())
            ++str;
        if (*str == QLatin1Char(','))
            ++str;

        //eat the rest of space
        while (str->isSpace())
            ++str;
    }

    return points;
}

static inline void parseNumbersArray(const QChar *&str, QVarLengthArray<qreal, 8> &points)
{
    while (str->isSpace())
        ++str;
    while (isDigit(str->unicode()) ||
           *str == QLatin1Char('-') || *str == QLatin1Char('+') ||
           *str == QLatin1Char('.')) {

        points.append(toDouble(str));

        while (str->isSpace())
            ++str;
        if (*str == QLatin1Char(','))
            ++str;

        //eat the rest of space
        while (str->isSpace())
            ++str;
    }
}

static QVector<qreal> parsePercentageList(const QChar *&str)
{
    QVector<qreal> points;
    if (!str)
        return points;

    while (str->isSpace())
        ++str;
    while ((*str >= QLatin1Char('0') && *str <= QLatin1Char('9')) ||
           *str == QLatin1Char('-') || *str == QLatin1Char('+') ||
           *str == QLatin1Char('.')) {

        points.append(toDouble(str));

        while (str->isSpace())
            ++str;
        if (*str == QLatin1Char('%'))
            ++str;
        while (str->isSpace())
            ++str;
        if (*str == QLatin1Char(','))
            ++str;

        //eat the rest of space
        while (str->isSpace())
            ++str;
    }

    return points;
}

static QString idFromUrl(const QString &url)
{
    QString::const_iterator itr = url.constBegin();
    while ((*itr).isSpace())
        ++itr;
    if ((*itr) == QLatin1Char('('))
        ++itr;
    while ((*itr).isSpace())
        ++itr;
    if ((*itr) == QLatin1Char('#'))
        ++itr;
    QString id;
    while ((*itr) != QLatin1Char(')')) {
        id += *itr;
        ++itr;
    }
    return id;
}

static inline QStringRef trimRef(const QStringRef &str)
{
    if (str.isEmpty())
        return QStringRef();
    const QChar *s = str.string()->constData() + str.position();
    int end = str.length() - 1;
    if (!s[0].isSpace() && !s[end].isSpace())
        return str;

    int start = 0;
    while (start<=end && s[start].isSpace())  // skip white space from start
        start++;
    if (start <= end) {                          // only white space
        while (s[end].isSpace())           // skip white space from end
            end--;
    }
    int l = end - start + 1;
    if (l <= 0)
        return QStringRef();
    return QStringRef(str.string(), str.position() + start, l);
}



static QMatrix parseTransformationMatrix(const QStringRef &value)
{
    if (value.isEmpty())
        return QMatrix();

    QMatrix matrix;
    const QChar *str = value.constData();
    const QChar *end = str + value.length();

    while (str < end) {
        if (str->isSpace() || *str == QLatin1Char(',')) {
            ++str;
            continue;
        }
        enum State {
            Matrix,
            Translate,
            Rotate,
            Scale,
            SkewX,
            SkewY
        };
        State state = Matrix;
        if (*str == QLatin1Char('m')) {  //matrix
            const char *ident = "atrix";
            for (int i = 0; i < 5; ++i)
                if (*(++str) != QLatin1Char(ident[i]))
                    goto error;
            ++str;
            state = Matrix;
        } else if (*str == QLatin1Char('t')) { //translate
            const char *ident = "ranslate";
            for (int i = 0; i < 8; ++i)
                if (*(++str) != QLatin1Char(ident[i]))
                    goto error;
            ++str;
            state = Translate;
        } else if (*str == QLatin1Char('r')) { //rotate
            const char *ident = "otate";
            for (int i = 0; i < 5; ++i)
                if (*(++str) != QLatin1Char(ident[i]))
                    goto error;
            ++str;
            state = Rotate;
        } else if (*str == QLatin1Char('s')) { //scale, skewX, skewY
            ++str;
            if (*str == QLatin1Char('c')) {
                const char *ident = "ale";
                for (int i = 0; i < 3; ++i)
                    if (*(++str) != QLatin1Char(ident[i]))
                        goto error;
                ++str;
                state = Scale;
            } else if (*str == QLatin1Char('k')) {
                if (*(++str) != QLatin1Char('e'))
                    goto error;
                if (*(++str) != QLatin1Char('w'))
                    goto error;
                ++str;
                if (*str == QLatin1Char('X'))
                    state = SkewX;
                else if (*str == QLatin1Char('Y'))
                    state = SkewY;
                else
                    goto error;
                ++str;
            } else {
                goto error;
            }
        } else {
            goto error;
        }


        while (str < end && str->isSpace())
            ++str;
        if (*str != QLatin1Char('('))
            goto error;
        ++str;
        QVarLengthArray<qreal, 8> points;
        parseNumbersArray(str, points);
        if (*str != QLatin1Char(')'))
            goto error;
        ++str;

        if(state == Matrix) {
            if(points.count() != 6)
                goto error;
            matrix = QMatrix(points[0], points[1],
                             points[2], points[3],
                             points[4], points[5]) * matrix;
        } else if (state == Translate) {
            if (points.count() == 1)
                matrix.translate(points[0], 0);
            else if (points.count() == 2)
                matrix.translate(points[0], points[1]);
            else
                goto error;
        } else if (state == Rotate) {
            if(points.count() == 1) {
                matrix.rotate(points[0]);
            } else if (points.count() == 3) {
                matrix.translate(points[1], points[2]);
                matrix.rotate(points[0]);
                matrix.translate(-points[1], -points[2]);
            } else {
                goto error;
            }
        } else if (state == Scale) {
            if (points.count() < 1 || points.count() > 2)
                goto error;
            qreal sx = points[0];
            qreal sy = sx;
            if(points.count() == 2)
                sy = points[1];
            matrix.scale(sx, sy);
        } else if (state == SkewX) {
            if (points.count() != 1)
                goto error;
            const qreal deg2rad = qreal(0.017453292519943295769);
            matrix.shear(qTan(points[0]*deg2rad), 0);
        } else if (state == SkewY) {
            if (points.count() != 1)
                goto error;
            const qreal deg2rad = qreal(0.017453292519943295769);
            matrix.shear(0, qTan(points[0]*deg2rad));
        }
    }
  error:
    return matrix;
}


static void pathArcSegment(QPainterPath &path,
                           qreal xc, qreal yc,
                           qreal th0, qreal th1,
                           qreal rx, qreal ry, qreal xAxisRotation)
{
    qreal sinTh, cosTh;
    qreal a00, a01, a10, a11;
    qreal x1, y1, x2, y2, x3, y3;
    qreal t;
    qreal thHalf;

    sinTh = qSin(xAxisRotation * (Q_PI / 180.0));
    cosTh = qCos(xAxisRotation * (Q_PI / 180.0));

    a00 =  cosTh * rx;
    a01 = -sinTh * ry;
    a10 =  sinTh * rx;
    a11 =  cosTh * ry;

    thHalf = 0.5 * (th1 - th0);
    t = (8.0 / 3.0) * qSin(thHalf * 0.5) * qSin(thHalf * 0.5) / qSin(thHalf);
    x1 = xc + qCos(th0) - t * qSin(th0);
    y1 = yc + qSin(th0) + t * qCos(th0);
    x3 = xc + qCos(th1);
    y3 = yc + qSin(th1);
    x2 = x3 + t * qSin(th1);
    y2 = y3 - t * qCos(th1);

    qDebug() << "cubicTo(" << a00 * x1 + a01 * y1 << "," << a10 * x1 + a11 * y1 << "," <<
                              a00 * x2 + a01 * y2 << "," << a10 * x2 + a11 * y2 << "," <<
                              a00 * x3 + a01 * y3 << "," << a10 * x3 + a11 * y3 << ");";
    path.cubicTo(a00 * x1 + a01 * y1, a10 * x1 + a11 * y1,
                 a00 * x2 + a01 * y2, a10 * x2 + a11 * y2,
                 a00 * x3 + a01 * y3, a10 * x3 + a11 * y3);
}

// the arc handling code underneath is from XSVG (BSD license)
/*
 * Copyright  2002 USC/Information Sciences Institute
 *
 * Permission to use, copy, modify, distribute, and sell this software
 * and its documentation for any purpose is hereby granted without
 * fee, provided that the above copyright notice appear in all copies
 * and that both that copyright notice and this permission notice
 * appear in supporting documentation, and that the name of
 * Information Sciences Institute not be used in advertising or
 * publicity pertaining to distribution of the software without
 * specific, written prior permission.  Information Sciences Institute
 * makes no representations about the suitability of this software for
 * any purpose.  It is provided "as is" without express or implied
 * warranty.
 *
 * INFORMATION SCIENCES INSTITUTE DISCLAIMS ALL WARRANTIES WITH REGARD
 * TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL INFORMATION SCIENCES
 * INSTITUTE BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA
 * OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 *
 */
static void pathArc(QPainterPath &path,
                    qreal               rx,
                    qreal               ry,
                    qreal               x_axis_rotation,
                    int         large_arc_flag,
                    int         sweep_flag,
                    qreal               x,
                    qreal               y,
                    qreal curx, qreal cury)
{
    qreal sin_th, cos_th;
    qreal a00, a01, a10, a11;
    qreal x0, y0, x1, y1, xc, yc;
    qreal d, sfactor, sfactor_sq;
    qreal th0, th1, th_arc;
    int i, n_segs;
    qreal dx, dy, dx1, dy1, Pr1, Pr2, Px, Py, check;

    rx = qAbs(rx);
    ry = qAbs(ry);

    sin_th = qSin(x_axis_rotation * (Q_PI / 180.0));
    cos_th = qCos(x_axis_rotation * (Q_PI / 180.0));

    dx = (curx - x) / 2.0;
    dy = (cury - y) / 2.0;
    dx1 =  cos_th * dx + sin_th * dy;
    dy1 = -sin_th * dx + cos_th * dy;
    Pr1 = rx * rx;
    Pr2 = ry * ry;
    Px = dx1 * dx1;
    Py = dy1 * dy1;
    /* Spec : check if radii are large enough */
    check = Px / Pr1 + Py / Pr2;
    if (check > 1) {
        rx = rx * qSqrt(check);
        ry = ry * qSqrt(check);
    }

    a00 =  cos_th / rx;
    a01 =  sin_th / rx;
    a10 = -sin_th / ry;
    a11 =  cos_th / ry;
    x0 = a00 * curx + a01 * cury;
    y0 = a10 * curx + a11 * cury;
    x1 = a00 * x + a01 * y;
    y1 = a10 * x + a11 * y;
    /* (x0, y0) is current point in transformed coordinate space.
       (x1, y1) is new point in transformed coordinate space.

       The arc fits a unit-radius circle in this space.
    */
    d = (x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0);
    sfactor_sq = 1.0 / d - 0.25;
    if (sfactor_sq < 0) sfactor_sq = 0;
    sfactor = qSqrt(sfactor_sq);
    if (sweep_flag == large_arc_flag) sfactor = -sfactor;
    xc = 0.5 * (x0 + x1) - sfactor * (y1 - y0);
    yc = 0.5 * (y0 + y1) + sfactor * (x1 - x0);
    /* (xc, yc) is center of the circle. */

    th0 = qAtan2(y0 - yc, x0 - xc);
    th1 = qAtan2(y1 - yc, x1 - xc);

    th_arc = th1 - th0;
    if (th_arc < 0 && sweep_flag)
        th_arc += 2 * Q_PI;
    else if (th_arc > 0 && !sweep_flag)
        th_arc -= 2 * Q_PI;

    n_segs = qCeil(qAbs(th_arc / (Q_PI * 0.5 + 0.001)));

    for (i = 0; i < n_segs; i++) {
        pathArcSegment(path, xc, yc,
                       th0 + i * th_arc / n_segs,
                       th0 + (i + 1) * th_arc / n_segs,
                       rx, ry, x_axis_rotation);
    }
}

static bool parsePathDataFast(const QStringRef &dataStr, QPainterPath &path)
{
    qreal x0 = 0, y0 = 0;              // starting point
    qreal x = 0, y = 0;                // current point
    char lastMode = 0;
    QPointF ctrlPt;
    const QChar *str = dataStr.constData();
    const QChar *end = str + dataStr.size();

    while (str != end) {
        while (str->isSpace())
            ++str;
        QChar pathElem = *str;
        ++str;
        QChar endc = *end;
        *const_cast<QChar *>(end) = 0; // parseNumbersArray requires 0-termination that QStringRef cannot guarantee
        QVarLengthArray<qreal, 8> arg;
        parseNumbersArray(str, arg);
        *const_cast<QChar *>(end) = endc;
        if (pathElem == QLatin1Char('z') || pathElem == QLatin1Char('Z'))
            arg.append(0);//dummy
        const qreal *num = arg.constData();
        int count = arg.count();
        while (count > 0) {
            qreal offsetX = x;        // correction offsets
            qreal offsetY = y;        // for relative commands
            switch (pathElem.unicode()) {
            case 'm': {
                if (count < 2) {
                    num++;
                    count--;
                    break;
                }
                x = x0 = num[0] + offsetX;
                y = y0 = num[1] + offsetY;
                num += 2;
                count -= 2;
                qDebug() << "a1.moveTo(" << x0 << "," << y0 << ");";
                path.moveTo(x0, y0);

                 // As per 1.2  spec 8.3.2 The "moveto" commands
                 // If a 'moveto' is followed by multiple pairs of coordinates without explicit commands,
                 // the subsequent pairs shall be treated as implicit 'lineto' commands.
                 pathElem = QLatin1Char('l');
            }
                break;
            case 'M': {
                if (count < 2) {
                    num++;
                    count--;
                    break;
                }
                x = x0 = num[0];
                y = y0 = num[1];
                num += 2;
                count -= 2;
                qDebug() << "a1.moveTo(" << x0 << "," << y0 << ");";
                path.moveTo(x0, y0);

                // As per 1.2  spec 8.3.2 The "moveto" commands
                // If a 'moveto' is followed by multiple pairs of coordinates without explicit commands,
                // the subsequent pairs shall be treated as implicit 'lineto' commands.
                pathElem = QLatin1Char('L');
            }
                break;
            case 'z':
            case 'Z': {
                x = x0;
                y = y0;
                count--; // skip dummy
                num++;
                qDebug() << "a1.closeSubpath();";
                path.closeSubpath();
            }
                break;
            case 'l': {
                if (count < 2) {
                    num++;
                    count--;
                    break;
                }
                x = num[0] + offsetX;
                y = num[1] + offsetY;
                num += 2;
                count -= 2;
                qDebug() << "a1.lineTo(" << x << "," << y << ");";
                path.lineTo(x, y);

            }
                break;
            case 'L': {
                if (count < 2) {
                    num++;
                    count--;
                    break;
                }
                x = num[0];
                y = num[1];
                num += 2;
                count -= 2;
                qDebug() << "a1.lineTo(" << x << "," << y << ");";
                path.lineTo(x, y);
            }
                break;
            case 'h': {
                x = num[0] + offsetX;
                num++;
                count--;
                qDebug() << "a1.lineTo(" << x << "," << y << ");";
                path.lineTo(x, y);
            }
                break;
            case 'H': {
                x = num[0];
                num++;
                count--;
                qDebug() << "a1.lineTo(" << x << "," << y << ");";
                path.lineTo(x, y);
            }
                break;
            case 'v': {
                y = num[0] + offsetY;
                num++;
                count--;
                qDebug() << "a1.lineTo(" << x << "," << y << ");";
                path.lineTo(x, y);
            }
                break;
            case 'V': {
                y = num[0];
                num++;
                count--;
                qDebug() << "a1.lineTo(" << x << "," << y << ");";
                path.lineTo(x, y);
            }
                break;
            case 'c': {
                if (count < 6) {
                    num += count;
                    count = 0;
                    break;
                }
                QPointF c1(num[0] + offsetX, num[1] + offsetY);
                QPointF c2(num[2] + offsetX, num[3] + offsetY);
                QPointF e(num[4] + offsetX, num[5] + offsetY);
                num += 6;
                count -= 6;
                qDebug() << "a1.cubicTo(" << c1.x() << "," << c1.y() << ","
                                          << c2.x() << "," << c2.y() << ","
                                          << e.x() << "," << e.y() << ");";
                path.cubicTo(c1, c2, e);
                ctrlPt = c2;
                x = e.x();
                y = e.y();
                break;
            }
            case 'C': {
                if (count < 6) {
                    num += count;
                    count = 0;
                    break;
                }
                QPointF c1(num[0], num[1]);
                QPointF c2(num[2], num[3]);
                QPointF e(num[4], num[5]);
                num += 6;
                count -= 6;
                qDebug() << "a1.cubicTo(" << c1.x() << "," << c1.y() << ","
                                          << c2.x() << "," << c2.y() << ","
                                          << e.x() << "," << e.y() << ");";
                path.cubicTo(c1, c2, e);
                ctrlPt = c2;
                x = e.x();
                y = e.y();
                break;
            }
            case 's': {
                if (count < 4) {
                    num += count;
                    count = 0;
                    break;
                }
                QPointF c1;
                if (lastMode == 'c' || lastMode == 'C' ||
                    lastMode == 's' || lastMode == 'S')
                    c1 = QPointF(2*x-ctrlPt.x(), 2*y-ctrlPt.y());
                else
                    c1 = QPointF(x, y);
                QPointF c2(num[0] + offsetX, num[1] + offsetY);
                QPointF e(num[2] + offsetX, num[3] + offsetY);
                num += 4;
                count -= 4;
                qDebug() << "a1.cubicTo(" << c1.x() << "," << c1.y() << ","
                                          << c2.x() << "," << c2.y() << ","
                                          << e.x() << "," << e.y() << ");";
                path.cubicTo(c1, c2, e);
                ctrlPt = c2;
                x = e.x();
                y = e.y();
                break;
            }
            case 'S': {
                if (count < 4) {
                    num += count;
                    count = 0;
                    break;
                }
                QPointF c1;
                if (lastMode == 'c' || lastMode == 'C' ||
                    lastMode == 's' || lastMode == 'S')
                    c1 = QPointF(2*x-ctrlPt.x(), 2*y-ctrlPt.y());
                else
                    c1 = QPointF(x, y);
                QPointF c2(num[0], num[1]);
                QPointF e(num[2], num[3]);
                num += 4;
                count -= 4;
                qDebug() << "a1.cubicTo(" << c1.x() << "," << c1.y() << ","
                                          << c2.x() << "," << c2.y() << ","
                                          << e.x() << "," << e.y() << ");";
                path.cubicTo(c1, c2, e);
                ctrlPt = c2;
                x = e.x();
                y = e.y();
                break;
            }
            case 'q': {
                if (count < 4) {
                    num += count;
                    count = 0;
                    break;
                }
                QPointF c(num[0] + offsetX, num[1] + offsetY);
                QPointF e(num[2] + offsetX, num[3] + offsetY);
                num += 4;
                count -= 4;
                qDebug() << "a1.quadTo(" << c.x() << "," << c.y() << ","
                                         << e.x() << "," << e.y() << ");";
                path.quadTo(c, e);
                ctrlPt = c;
                x = e.x();
                y = e.y();
                break;
            }
            case 'Q': {
                if (count < 4) {
                    num += count;
                    count = 0;
                    break;
                }
                QPointF c(num[0], num[1]);
                QPointF e(num[2], num[3]);
                num += 4;
                count -= 4;
                qDebug() << "a1.quadTo(" << c.x() << "," << c.y() << ","
                                         << e.x() << "," << e.y() << ");";
                path.quadTo(c, e);
                ctrlPt = c;
                x = e.x();
                y = e.y();
                break;
            }
            case 't': {
                if (count < 2) {
                    num += count;
                    count = 0;
                    break;
                }
                QPointF e(num[0] + offsetX, num[1] + offsetY);
                num += 2;
                count -= 2;
                QPointF c;
                if (lastMode == 'q' || lastMode == 'Q' ||
                    lastMode == 't' || lastMode == 'T')
                    c = QPointF(2*x-ctrlPt.x(), 2*y-ctrlPt.y());
                else
                    c = QPointF(x, y);
                qDebug() << "a1.quadTo(" << c.x() << "," << c.y() << ","
                                         << e.x() << "," << e.y() << ");";
                path.quadTo(c, e);
                ctrlPt = c;
                x = e.x();
                y = e.y();
                break;
            }
            case 'T': {
                if (count < 2) {
                    num += count;
                    count = 0;
                    break;
                }
                QPointF e(num[0], num[1]);
                num += 2;
                count -= 2;
                QPointF c;
                if (lastMode == 'q' || lastMode == 'Q' ||
                    lastMode == 't' || lastMode == 'T')
                    c = QPointF(2*x-ctrlPt.x(), 2*y-ctrlPt.y());
                else
                    c = QPointF(x, y);
                qDebug() << "a1.quadTo(" << c.x() << "," << c.y() << ","
                                         << e.x() << "," << e.y() << ");";
                path.quadTo(c, e);
                ctrlPt = c;
                x = e.x();
                y = e.y();
                break;
            }
            case 'a': {
                if (count < 7) {
                    num += count;
                    count = 0;
                    break;
                }
                qreal rx = (*num++);
                qreal ry = (*num++);
                qreal xAxisRotation = (*num++);
                qreal largeArcFlag  = (*num++);
                qreal sweepFlag = (*num++);
                qreal ex = (*num++) + offsetX;
                qreal ey = (*num++) + offsetY;
                count -= 7;
                qreal curx = x;
                qreal cury = y;
                pathArc(path, rx, ry, xAxisRotation, int(largeArcFlag),
                        int(sweepFlag), ex, ey, curx, cury);

                x = ex;
                y = ey;
            }
                break;
            case 'A': {
                if (count < 7) {
                    num += count;
                    count = 0;
                    break;
                }
                qreal rx = (*num++);
                qreal ry = (*num++);
                qreal xAxisRotation = (*num++);
                qreal largeArcFlag  = (*num++);
                qreal sweepFlag = (*num++);
                qreal ex = (*num++);
                qreal ey = (*num++);
                count -= 7;
                qreal curx = x;
                qreal cury = y;
                pathArc(path, rx, ry, xAxisRotation, int(largeArcFlag),
                        int(sweepFlag), ex, ey, curx, cury);

                x = ex;
                y = ey;
            }
                break;
            default:
                return false;
            }
            lastMode = pathElem.toLatin1();
        }
    }
    return true;
}





void svgParse()
  {

  QString src("m 29.237991,-32.55304 c 0.08328,5.107819 0.557859,6.384957 2.221211,10.215637 2.178075,5.017529 10.977646,9.121266 15.800767,17.6949758 4.823446,8.5737111 4.685794,13.551144 4.685794,18.6586042 0,5.108178 -1.028447,8.83376 -1.028447,8.83376 0,0 0.196926,-0.654099 -0.78113,-3.163584 0.332937,-3.283857 0.63788,-9.43621 -3.375129,-17.9441861 -1.820109,-3.859579 -10.097391,-10.3449411 -17.582339,-15.8175529 0.08328,5.1078187 0.557849,6.3849554 2.22121,10.2156378 2.178075,5.0175228 10.977646,9.1212607 15.800768,17.6949722 4.823438,8.57371 4.685785,13.551143 4.685785,18.658602 0,5.10818 -1.02844,8.833761 -1.02844,8.833761 0,0 0.196928,-0.654099 -0.781129,-3.163584 0.332937,-3.283855 0.63788,-9.43621 -3.375129,-17.944185 C 44.881673,16.360239 36.66334,9.874877 29.178382,4.4022629 c 0.166309,6.2025611 0.724158,8.3382221 3.385343,12.3513021 3.640882,5.49067 9.813523,8.756468 14.802944,16.053405 4.989748,7.296574 5.519277,18.900233 3.373804,26.362589 0,0 -0.391549,-1.114604 -0.947756,-2.398964 1.636681,-21.677352 -13.012145,-26.387148 -20.614662,-33.365526 0,9.121263 3.05306,13.962169 13.365166,23.630618 10.312098,9.668813 10.266986,19.775736 10.302219,24.70116 0.05896,8.214702 -3.521335,15.094831 -5.31247,17.621291 0.665212,-2.553909 6.662652,-19.304755 0.498911,-29.005348 -3.124525,-4.917841 -10.977645,-7.662092 -18.794878,-14.593875 0,0 0.166301,32.516751 0,48.205367 -3.991935,11.492778 -13.638821,13.636378 -18.628569,13.636378 -4.9897488,0 -10.02461925,-3.25821 -10.14613344,-7.661715 C 0.31180248,94.480404 4.1212981,89.54054 10.275823,86.257039 16.430021,82.973182 24.413555,82.790791 27.241366,85.345056 l -0.0098,-137.397167 2.005842,-0.02168 0.05896,4.799365 c 0.02141,1.776291 0.204836,3.237624 2.269952,5.781418 3.122221,3.846218 9.31462,9.486057 13.63883,15.141064 4.324862,5.655009 6.154524,11.144237 6.525662,16.265058 0.439965,6.075423 -1.036676,13.6526364 -1.036676,13.6526364 0,0 0.107357,-0.5876424 -0.72416,-2.59472209 C 50.117835,-11.966473 48.365889,-13.582397 44.540264,-19.419803 40.71464,-25.257207 36.889341,-27.445601 29.238082,-32.553062");
  QPainterPath path;
  parsePathDataFast( QStringRef( &src ), path );
  qDebug() << path;
  }



void svgPaint(QPainter *painter , int w)
  {
  QPainterPath a1,a2;
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

  QTransform t1;
  auto af2 = a1.toFillPolygon( t1 );


  QMatrix scale;
  double sc = static_cast<double>(w) / 60.0;
  scale.scale( sc, sc );

  painter->setRenderHint( QPainter::Antialiasing, true );
  painter->setBrush( QBrush(Qt::black) );
//  for( const auto &poly : af2 ) {
//    painter->drawPolygon( scale.map( poly ) );
//    }
  //painter->drawPath( scale.map(a2) );
  painter->setPen( Qt::transparent );
  painter->drawPolygon( scale.map(af2) );


#if 0
  qDebug() << "width" << w;
  QPainterPath a1;
  a1.moveTo( 113.134, 900.473 );
  a1.lineTo( 113.134, 983.613 );

  QMatrix t2;
  t2.translate( -84.221634,-840.67061 );

  QPainterPath a2;
  a2.moveTo( 153.038 , 108.771 );
  a2.cubicTo( 153.038 , 112.641 , 148.414 , 115.844 , 142.484 , 116.079 );
  a2.cubicTo( 136.554 , 116.315 , 131.367 , 113.503 , 130.646 , 109.66 );
  a2.cubicTo( 129.924 , 105.818 , 133.916 , 102.274 , 139.758 , 101.57 );
  a2.cubicTo( 145.6 , 100.867 , 151.273 , 103.248 , 152.706 , 107.004 );

  QMatrix t1( 1.2477873,-0.63522684,0.59622185,1.3694102,-142.33379,929.29558 );
  QPolygonF af2 = a2.toFillPolygon( t1 );

  a2 = t1.map( a2 );

  a1 = t2.map( a1 );
  a2 = t2.map( a2 );
  af2 = t2.map( af2 );


  QMatrix scale;
  double sc = static_cast<double>(w) / 60.0;
  qDebug() << "scale" << sc;
  scale.scale( sc, sc );
  a1 = scale.map( a1 );
  af2 = scale.map( af2 );

  qDebug() << a1 << a2;

  painter->setRenderHint( QPainter::Antialiasing, true );
  painter->drawPath( a1 );
  painter->setBrush( QBrush(Qt::black) );
  painter->drawPolygon( af2 );
#endif
  }
