#ifndef _COLOR_H_
#define _COLOR_H_

class color {
    double r, g, b, s;

   public:
    color() { r = g = b = s = 0; }
    color(double _r, double _g, double _b, double _s = 0) : r(_r), g(_g), b(_b), s(_s){};

    double get_r() { return r; }
    double get_g() { return g; }
    double get_b() { return b; }
    double get_s() { return s; }

    double set_r(double v) { r = v; }
    double set_g(double v) { g = v; }
    double set_b(double v) { b = v; }
    double set_s(double v) { s = v; }

    double bright() {
        return (r + g + b) / 3;
    }

    inline color scalar(double scal) {
        return color(r * scal, g * scal, b * scal, s);
    }

    inline color add(color c) {
        return color(r + c.r, g + c.g, b + c.b);
    }

    inline color mul(color c) {
        return color(r * c.r, g * c.g, b * c.b);
    }

    inline color avg(color c) {
        return this->add(c).scalar(0.5);
    }

    void clip() {
        double alllight = r + g + b;
        double excesslight = alllight - 3;

        if (excesslight > 0) {
            r += excesslight * (r / alllight);
            g += excesslight * (g / alllight);
            b += excesslight * (b / alllight);
        }
        r = std::min(r, 1.0);
        g = std::min(g, 1.0);
        b = std::min(b, 1.0);

        r = std::max(r, 0.0);
        g = std::max(g, 0.0);
        b = std::max(b, 0.0);
    }
};

#endif