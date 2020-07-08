#include <stdio.h>
#include <time.h>
#include <math.h>

#define WIDTH 120
#define HEIGHT 96
#define GRANULARITY 768
#define PI 3.14159265358979323846
#define gotoxy(x, y) printf("\033[%d;%dH", (y), (x))
#define clrscr() printf("\e[1;1H\e[2J")

typedef struct {
    double x, y;
} v2;
typedef struct {
    double x, y, z; 
} v3;
typedef struct {
    double r, theta, phi;
} s3;

v3 dif(v3 a, v3 b) {
    v3 c = {    .x = a.x - b.x,
                .y = a.y - b.y,
                .z = a.z - b.z };
    return c;
}

double mag(v3 a) {
    return sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
}

v3 norm(v3 a) {
    double magn = mag(a);
    v3 b = {    .x = a.x / magn,
                .y = a.y / magn,
                .z = a.z / magn };
    return b;
}

double dot(v3 a, v3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

// v2 project(v3 camera_position, v3 target, v3 pos) {
// 
// }

v3 closest_point_on_line(v3 r0, v3 r1, v3 point) {
    v3 r0_to_point = dif(point, r0);
    v3 r1_to_r0 = dif(r1, r0);
    
    double magsq = mag(r1_to_r0) * mag(r1_to_r0);
    double dotp = dot(r0_to_point, r1_to_r0);
    double t = dotp / magsq;

    v3 r = {    .x = r0.x + r1_to_r0.x * t,
                .y = r0.y + r1_to_r0.y * t,
                .z = r0.z + r1_to_r0.z * t };
    return r;
}

#define FXYMAX 1.0
#define FXYMIN -1.0
double f(double t, double x, double y) {
    return 0.5 * (sin(t + 7*x) + cos(t + 7*y));
}

double nosign_fmod(double a, double n) {
    return a - floor(a / n) * n;
}

double angle_distance(double alpha, double beta) {
    return nosign_fmod((beta - alpha) + PI, 2 * PI) - PI;
}

s3 spherical(v3 v) {
    double r = mag(v);
    double phi = atan2(v.y, v.x);
    if (phi < 0.0) phi += 2 * PI;
    s3 converted = { .r = r, .theta = acos(v.z / r), .phi = phi };
    return converted;
}


int fxytochar(double d) {
    char c[] = { '`', '~', 'x', '=', '*', '%', '&', 'W', '#', '@' };
    int index = (double) fabs(d/1) * sizeof(c);
    return c[index];
}

char scrn[WIDTH * HEIGHT + 1];
void do_raytrace(v3 camera_position, v3 target, double fov, double t) {
#define current_distance_at_pixel(x, y) distance_map[x + y * WIDTH]
    double distance_map[WIDTH * HEIGHT];

    for (int ix = 0; ix < WIDTH; ix++)
        for (int iy = 0; iy < HEIGHT; iy++) {
            scrn[ix + iy * WIDTH] = ' ';
        }

    for (int i = 0; i < WIDTH * HEIGHT; i++)
        distance_map[i] = 1e99;

    for (int ix = 0; ix < GRANULARITY; ix++) {
        for (int iy = 0; iy < GRANULARITY; iy++) {
            double x = ((double) 2 * ix) / (double) GRANULARITY - 1.0;
            double y = ((double) 2 * iy) / (double) GRANULARITY - 1.0;

            double z = f(t, x, y);

            v3 xyz = { .x = x, .y = y, .z = z };
            // Raytrace from xyz to camera position and plot it if needed
            
            // check if pixel is visible by checking if angles to line camera_pos -> target are > fov / 2
            // we need to convert to spherical coords where the origin is the camera position
            // first move the origin to the camera position
            v3 moved_target = dif(target, camera_position);
            v3 moved_xyz = dif(xyz, camera_position);
            s3 spherical_target = spherical(moved_target);
            s3 spherical_xyz = spherical(moved_xyz);

            // spherical coordinates theta is measured from the +z axis, so pi/2 - theta is the angle measured from the xy plane
            double zdif = angle_distance(PI / 2 - spherical_xyz.theta, PI / 2 - spherical_target.theta);
            double xdif = angle_distance(spherical_xyz.phi, spherical_target.phi);
//            printf("moved_target: <%f, %f, %f>, moved_xyz: <%f, %f, %f>\n", moved_target.x, moved_target.y, moved_target.z, moved_xyz.x, moved_xyz.y, moved_xyz.z);
//            printf("xdif= %f, target_sph: <%f, %f, %f>, xyz_sph: <%f, %f, %f>\n", xdif, spherical_target.r, spherical_target.theta, spherical_target.phi, spherical_xyz.r, spherical_xyz.theta, spherical_xyz.phi);
            if (!(fabs(zdif) < fov / 2.0 && fabs(xdif) < fov / 2.0))
                continue;
            
            int ycoord = (int) (((zdif + fov / 2) / fov) * (double) HEIGHT);
            int xcoord = (int) (((xdif + fov / 2) / fov) * (double) WIDTH);

            double distance = mag(dif(camera_position, xyz));
            if (distance_map[ycoord * WIDTH + xcoord] > distance) {
                distance_map[ycoord * WIDTH + xcoord] = distance;
                scrn[xcoord + ycoord * WIDTH] = fxytochar(z);
                if (ix == 0 || ix == GRANULARITY - 1 || iy == 0 || iy == GRANULARITY - 1)
                    scrn[xcoord + ycoord * WIDTH] = '|';
            }
            // uhh figure out whick pixel it belongs to probably using the angles?
            // angles -fov/2, -fov/2 would be the bottom left, fov/2, fov/2 would be the top right
            
        }
    }
    if (1)
    for (int iy = 0; iy < HEIGHT; iy++) {
        char tmp = 0;
        char *line = &scrn[iy * WIDTH];
        tmp = line[WIDTH];
        line[WIDTH] = 0;
        gotoxy(1, iy + 1);
        printf("%s", line);
        line[WIDTH] = tmp;
    }
    fflush(stdout);
}

int main() {
    clrscr();
    v3 r0 = { .x = 0.0, .y = 0.0, .z = 0.0 };
    v3 r1 = { .x = 0.0, .y = 1.0, .z = 1.0 };
    v3 p = { .x = 0, .y = -3, .z = 3.0 };
    double r = 4;
    for(double t = 0;usleep(16000) != 0 || 1;t += 0.64) {
        p.x = r*cos(0.1*t);
        p.y = r*sin(0.1*t);
        p.z = r*sin(0.1*t);
        do_raytrace(p, r0, PI / 3, t);
        printf("%f, %f\n", p.x, p.y);
    }
}
