/*********************************************************************************
 * Copyright (C) [2019] [houzh@msn.com]
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *********************************************************************************/
#include <porting/cdlog.h>
#include <drawables/hwpathmeasure.h>
namespace cdroid {
namespace hwui{

PathMeasure::PathMeasure(Cairo::RefPtr<cdroid::Path>inPath,bool){
    mPath=inPath;
}

double PathMeasure::distance(const Point& p1, const Point& p2) {
    const double dx = p2.x - p1.x;
    const double dy = p2.y - p1.y;
    return std::sqrt(dx*dx+dy*dy);
}

double PathMeasure::curveLength(const Point& p0, const Point& p1, const Point& p2, const Point& p3) {
    // Approximate the length of a cubic Bezier curve using a simple method
    double length = 0.0;
    Point prev = p0;
    const int steps = 36;
    for (int i = 1; i <= steps; ++i) {
        double t = static_cast<double>(i) / steps;
        Point point = interpolateCurve(p0, p1, p2, p3, t);
        length += distance(prev, point);
        prev = point;
    }
    return length;
}

PathMeasure::Point PathMeasure::interpolate(const Point& p1, const Point& p2, double t) {
    return { p1.x + t * (p2.x - p1.x), p1.y + t * (p2.y - p1.y) };
}

PathMeasure::Point PathMeasure::interpolateCurve(const Point& p0, const Point& p1, const Point& p2, const Point& p3, double t) {
    double u = 1.0 - t;
    double tt = t * t;
    double uu = u * u;
    double uuu = uu * u;
    double ttt = tt * t;

    Point p = {
        uuu * p0.x + 3.0 * uu * t * p1.x + 3.0 * u * tt * p2.x + ttt * p3.x,
        uuu * p0.y + 3.0 * uu * t * p1.y + 3.0 * u * tt * p2.y + ttt * p3.y
    };
    return p;
}

#define CURVE_SKIP_POINTS 4
double PathMeasure::getLength() {
    Point first_point,last_point;
    double length =0;
    auto m_path=mPath->copy_path();
    for (int i = 0; i < m_path->num_data; ) {
        cairo_path_data_t* data = &m_path->data[i];
        switch (data->header.type) {
        case CAIRO_PATH_MOVE_TO:
            last_point = { data[1].point.x, data[1].point.y };
            first_point=last_point;
            i += 2;
            break;
        case CAIRO_PATH_LINE_TO: {
            Point current_point = { data[1].point.x, data[1].point.y };
            length += distance(last_point, current_point);
            last_point = current_point;
            i += 2;
            break;
        }
        case CAIRO_PATH_CURVE_TO: {
            Point p1 = { data[1].point.x, data[1].point.y };
            Point p2 = { data[2].point.x, data[2].point.y };
            Point p3 = { data[3].point.x, data[3].point.y };
            length += curveLength(last_point, p1, p2, p3);
            last_point = p3;
            i += CURVE_SKIP_POINTS;
            break;
        }
        case CAIRO_PATH_CLOSE_PATH:
            length += distance(last_point,first_point);
            i += 1;
            break;
        default:    break;
        }
    }
    cairo_path_destroy(m_path);
    return length;
}

bool PathMeasure::getSegment(double startD, double stopD, Cairo::RefPtr<cdroid::Path>& dst, bool startWithMoveTo) {
    double length = 0.0,segment_length;
    Point p1 , p2 , p3;
    Point first_point,last_point,current_point;
    bool segment_started = false,segment_found = false;
    auto m_path = mPath->copy_path();
    for (int i = 0; i < m_path->num_data && (length<stopD); ) {
        cairo_path_data_t* data = &m_path->data[i];
        switch (data->header.type) {
        case CAIRO_PATH_MOVE_TO:
            last_point = { data[1].point.x, data[1].point.y };
            first_point = last_point;
            if (startWithMoveTo && !segment_started && length >= startD) {
                dst->move_to(last_point.x, last_point.y);
                segment_started = true;
            }
            i += 2;
            break;
        case CAIRO_PATH_LINE_TO:
            current_point = { data[1].point.x, data[1].point.y };
            segment_length = distance(last_point, current_point);
            if (length + segment_length >= startD && length <= stopD) {
                if (!segment_started) {
                    Point p =interpolate(last_point, current_point, (startD - length) / segment_length);
                    dst->move_to(p.x,p.y);
                    segment_started = true;
                }
                if (length + segment_length > stopD) {
                    Point p = interpolate(last_point, current_point, (stopD - length) / segment_length);
                    dst->line_to(p.x,p.y);
                    current_point = p;
                    segment_found = true ;
                } else {
                    dst->line_to(current_point.x, current_point.y);
                }
            }
            length += segment_length;
            last_point = current_point;
            i += 2;
            break;
        case CAIRO_PATH_CURVE_TO:
            p1 = { data[1].point.x, data[1].point.y };
            p2 = { data[2].point.x, data[2].point.y };
            p3 = { data[3].point.x, data[3].point.y };
            segment_length = curveLength(last_point, p1, p2, p3);
            if (length + segment_length >= startD && length <= stopD) {
                if (!segment_started) {
                    Point p = interpolateCurve(last_point, p1, p2, p3, (startD - length)/segment_length);
                    dst->move_to(p.x,p.y);
                    LOGV("(%f,%f),(%f,%f),(%f,%f),(%f,%f),(%f,%f),(%f,%f)",last_point.x,last_point.y,p1.x, p1.y, p2.x, p2.y,p3.x,p3.y,p.x,p.y);
                    segment_started = true;
                }
                if (length + segment_length > stopD) {
                    Point p = interpolateCurve(last_point, p1, p2, p3, (stopD - length) / segment_length);
                    dst->curve_to(p1.x, p1.y, p2.x, p2.y,p.x,p.y);
                    segment_found = true;
                    LOGV("(%f,%f),(%f,%f),(%f,%f),(%f,%f),(%f,%f)",p1.x, p1.y, p2.x, p2.y,p3.x,p3.y,p.x,p.y);
                } else {
                    dst->curve_to(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);
                }
            }
            length += segment_length;
            LOGV("segment[%d/%d]=%f/%f",i,m_path->num_data,stopD-startD,length);
            i += CURVE_SKIP_POINTS;
            last_point = p3;
            break;
        case CAIRO_PATH_CLOSE_PATH:
            segment_length = distance(last_point, first_point);
            if (length + segment_length >= startD && length <= stopD) {
                if (!segment_started) {
                    Point p = interpolate(last_point, first_point, (startD - length) / segment_length);
                    dst->move_to(p.x,p.y);
                    segment_started = true;
                }
                if (length + segment_length > stopD) {
                    Point p = interpolate(last_point, first_point, (stopD - length) / segment_length);
                    dst->line_to(p.x,p.y);
                    segment_found = true;
                } else {
                    dst->line_to(first_point.x, first_point.y);
                }
            }
            length += segment_length;
            last_point = first_point;
            i += 1;
            break;
        default:  break;
        }
    }
    cairo_path_destroy(m_path);
    return true;
}

}/**/
}/**/
