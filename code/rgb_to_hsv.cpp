#include "rgb_to_hsv.h"


struct hsv_color rgb_to_hsv(struct rgb_color rgb) {
    struct hsv_color hsv;
    double rgb_min, rgb_max;
    rgb_min = MIN3(rgb.r, rgb.g, rgb.b);
    rgb_max = MAX3(rgb.r, rgb.g, rgb.b);
    hsv.val = rgb_max;
    if (hsv.val == 0) {
        hsv.hue = hsv.sat = 0;
        return hsv;
    }
    /* Normalize value to 1 */
    rgb.r /= hsv.val;
    rgb.g /= hsv.val;
    rgb.b /= hsv.val;
    rgb_min = MIN3(rgb.r, rgb.g, rgb.b);
    rgb_max = MAX3(rgb.r, rgb.g, rgb.b);
    hsv.sat = rgb_max - rgb_min;
    if (hsv.sat == 0) {
        hsv.hue = 0;
        return hsv;
    }
    /* Normalize saturation to 1 */
    rgb.r = (rgb.r - rgb_min)/(rgb_max - rgb_min);
    rgb.g = (rgb.g - rgb_min)/(rgb_max - rgb_min);
    rgb.b = (rgb.b - rgb_min)/(rgb_max - rgb_min);
    rgb_min = MIN3(rgb.r, rgb.g, rgb.b);
    rgb_max = MAX3(rgb.r, rgb.g, rgb.b);
    /* Compute hue */
    if (rgb_max == rgb.r) {
        hsv.hue = 0.0 + 60.0*(rgb.g - rgb.b);
        if (hsv.hue < 0.0) {
            hsv.hue += 360.0;
        }
    } else if (rgb_max == rgb.g) {
        hsv.hue = 120.0 + 60.0*(rgb.b - rgb.r);
    } else /* rgb_max == rgb.b */ {
        hsv.hue = 240.0 + 60.0*(rgb.r - rgb.g);
    }
    return hsv;
}
