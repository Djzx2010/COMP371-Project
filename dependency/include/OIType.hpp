//
// Created by Haotao Lai on 2018-10-12.
// Created by Jashanjot Singh (OIRGBPixel, OIDepthPixel)
//

#ifndef OPENISS_TYPE_H
#define OPENISS_TYPE_H

namespace openiss
{

typedef struct DistortionMode
{

} DistortionMode;

typedef struct Intrinsic
{
    int width{0};
    int height{0};
    float cx{0};
    float cy{0};
    float fx{0};
    float fy{0};
    float coeffs[5]{};

    Intrinsic() = default;
    Intrinsic(const Intrinsic& tmp) {
        width = tmp.width;
        height = tmp.height;
        cx = tmp.cx;
        cy = tmp.cy;
        fx = tmp.fx;
        fy = tmp.fy;
        for (int i = 0; i < 5; i++) coeffs[i] = tmp.coeffs[i];
    }
} Intrinsic;

typedef struct Extrinsic
{
    // extrinsic matrix represents as follow

    // r0 r3 r6 | t0
    // r1 r4 r7 | t1
    // r2 r5 r9 | t2

    float rotation[9]{0};
    float translation[3]{0};

    Extrinsic() = default;
    Extrinsic(const Extrinsic& tmp) {
        for (int i = 0; i < 9; i++) rotation[i] = tmp.rotation[i];
        for (int i = 0; i < 3; i++) translation[i] = tmp.translation[i];
    }
} Extrinsic;

typedef struct Point2i
{
    int x{0};
    int y{0};
} Point2i;

typedef struct Point2f
{
    float x{0};
    float y{0};
} Point2f;

typedef struct Point3f
{
    float x{0};
    float y{0};
    float z{0};
} Point3f;

/**/
typedef struct
{
    /* Red value of this pixel. */
    uint8_t r;
    /* Green value of this pixel. */
    uint8_t g;
    /* Blue value of this pixel. */
    uint8_t b;
} OIRGBPixel;

/**/
typedef uint16_t OIDepthPixel;

// this template is borrowed from openni2
template<class T>
class Array
{
public:
    Array() : m_data(NULL), m_count(0), m_owner(false) {}
    Array(const T *data, int count) : m_owner(false) { _setData(data, count); }
    ~Array() { clear(); }

    int getSize() const { return m_count; }
    const T &operator[](int index) const { return m_data[index]; }
    void _setData(const T *data, int count, bool isOwner = false) {
        clear();
        m_count = count;
        m_owner = isOwner;
        if (!isOwner) {
            m_data = data;
        }
        else {
            m_data = new T[count];
            memcpy((void *) m_data, data, count * sizeof(T));
        }
    }

private:
    Array(const Array<T> &);
    Array<T> &operator=(const Array<T> &);

    void clear() {
        if (m_owner && m_data != NULL) {
            delete[] m_data;
        }
        m_owner = false;
        m_data = NULL;
        m_count = 0;
    }

    const T *m_data;
    int m_count;
    bool m_owner;
};

} // end of namespace

#endif // OPENISS_TYPE_H
