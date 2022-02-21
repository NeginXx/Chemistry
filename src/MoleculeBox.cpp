#include <math.h>
#include <stdio.h>
#include "../include/MoleculeBox.h"

const float kMinEnergyToReact = 500.0f;
const float kUniGasConstant = 8.314f;

extern Color kCircleColor;
extern Color kRectangleColor;

MoleculeBox::MoleculeBox(const Point2D<float>& left_corner,
                         const Point2D<float>& right_corner,
                         const Color& frame_color,
                         const Color& background_color)
  : left_corner_(left_corner), right_corner_(right_corner),
    frame_color_(frame_color), background_color_(background_color)
{
  Point2D<float>& l_c = left_corner_;
  Point2D<float>& r_c = right_corner_;
  Point2D<float> ld_c = {l_c.x, r_c.y};
  Point2D<float> ru_c = {r_c.x, l_c.y};
  // Point2D<float> ld_c = {l_c.x + 3, r_c.y + 3};
  // Point2D<float> ru_c = {r_c.x - 1, l_c.y - 2};
  // Color& color = frame_color_;
  Color color = {0, 0, 0};
  typedef PhysicalWall Wall;
  Wall* up_wall    = new Wall(l_c, ru_c, color, 10.0f);
  Wall* left_wall  = new Wall(l_c, ld_c, color, 10.0f);
  Wall* right_wall = new Wall(ru_c, r_c, color, 10.0f);
  Wall* down_wall  = new Wall(ld_c, r_c, color, 10.0f);
  shapes_.PushFront(up_wall);
  shapes_.PushFront(left_wall);
  shapes_.PushFront(right_wall);
  shapes_.PushFront(down_wall);
}

MoleculeBox::~MoleculeBox() {
	for (auto it = shapes_.begin(); it != shapes_.end(); ++it) {
    delete *it;
	}
}

void ProcessTwoShapesWW(PhysicalWall* wall1, PhysicalWall* wall2) {}



namespace DoIntersect {
  bool CC(PhysicalCircle* crl1, PhysicalCircle* crl2) {
    Vec2D<float> dist = crl2->GetCenter() - crl1->GetCenter();
    return dist.Length() < crl1->GetRadius() + crl2->GetRadius();
  }

  bool CR(PhysicalCircle* crl, PhysicalRectangle* rect) {
    float radius = crl->GetRadius();
    Vec2D<float> dist = rect->GetCenter() - crl->GetCenter();
    return fabs(dist.x) < rect->GetWidth() / 2 + radius &&
           fabs(dist.y) < rect->GetHeight() / 2 + radius;
  }

  bool CW(PhysicalCircle* crl, PhysicalWall* wall) {
    Vec2D<float> line_vec = Point2D<float>(wall->GetA() - wall->GetB());
    Vec2D<float> center_vec = crl->GetCenter();
    Vec2D<float> temp = center_vec - (Vec2D<float>)wall->GetB();
    return fabs(line_vec.x * temp.y - line_vec.y * temp.x) / line_vec.Length() < crl->GetRadius();
  }

  bool RR(PhysicalRectangle* rect1, PhysicalRectangle* rect2) {
    Vec2D<float> dist = rect1->GetCenter() - rect2->GetCenter();
    return fabs(dist.x) < rect1->GetWidth() / 2 + rect2->GetWidth() / 2 &&
           fabs(dist.y) < rect1->GetHeight() / 2 + rect2->GetHeight() / 2;
  }

  bool RW(PhysicalRectangle* rect, PhysicalWall* wall) {
    Vec2D<float> line_vec = Point2D<float>(wall->GetA() - wall->GetB());
    Vec2D<float> center_vec = rect->GetCenter();
    Vec2D<float> temp = center_vec - (Vec2D<float>)wall->GetB();
    float dist = fabs(line_vec.x * temp.y - line_vec.y * temp.x) / line_vec.Length();
    return dist < rect->GetWidth() / 2 || dist < rect->GetHeight() / 2;
  }

  bool WW(PhysicalWall* wall1, PhysicalWall* wall2) {
    return false;
  }
}

namespace IsEnergyEnough {
  float KineticEnergy(Vec2D<float> velocity, float mass) {
    return mass * velocity.SquaredLength() / 2;
  }

  bool CC(PhysicalCircle* crl1, PhysicalCircle* crl2) {
    return KineticEnergy(crl1->GetVelocity(), crl1->GetMass()) +
           KineticEnergy(crl2->GetVelocity(), crl2->GetMass()) > kMinEnergyToReact;
  }

  bool CR(PhysicalCircle* crl, PhysicalRectangle* rect) {
    return KineticEnergy(crl->GetVelocity(), crl->GetMass()) +
           KineticEnergy(rect->GetVelocity(), rect->GetMass()) > kMinEnergyToReact;
  }

  bool CW(PhysicalCircle* crl, PhysicalWall* wall) {
    return false;
  }

  bool RR(PhysicalRectangle* rect1, PhysicalRectangle* rect2) {
    return KineticEnergy(rect1->GetVelocity(), rect1->GetMass()) +
           KineticEnergy(rect2->GetVelocity(), rect2->GetMass()) > kMinEnergyToReact;
  }

  bool RW(PhysicalRectangle* rect, PhysicalWall* wall) {
    return false;
  }

  bool WW(PhysicalWall* wall1, PhysicalWall* wall2) {
    return false;
  }
}

namespace VelocitysUpdate {
  void CC(PhysicalCircle* crl1, PhysicalCircle* crl2) {
    Vec2D<float> temp = crl2->GetCenter() - crl1->GetCenter();
    Vec2D<float> aligned_vec = temp / temp.Length();
    
    float v1 = aligned_vec * crl1->GetVelocity();
    float v2 = aligned_vec * crl2->GetVelocity();
    float m1 = crl1->GetMass();
    float m2 = crl2->GetMass();
    float new_v1 = (2 * m2 * v2 + v1 * (m1 - m2)) / (m1 + m2);
    float new_v2 = (2 * m1 * v1 + v2 * (m2 - m1)) / (m1 + m2);

    Vec2D<float> v1_normal = crl1->GetVelocity() - aligned_vec * v1;
    Vec2D<float> v2_normal = crl2->GetVelocity() - aligned_vec * v2;
    crl1->SetVelocity(v1_normal + aligned_vec * new_v1);
    crl2->SetVelocity(v2_normal + aligned_vec * new_v2);
  }

  void CR(PhysicalCircle* crl, PhysicalRectangle* rect) {
    Vec2D<float> temp = crl->GetCenter() - rect->GetCenter();
    Vec2D<float> aligned_vec = temp / temp.Length();
    
    float v1 = aligned_vec * rect->GetVelocity();
    float v2 = aligned_vec * crl->GetVelocity();
    float m1 = rect->GetMass();
    float m2 = crl->GetMass();
    float new_v1 = (2 * m2 * v2 + v1 * (m1 - m2)) / (m1 + m2);
    float new_v2 = (2 * m1 * v1 + v2 * (m2 - m1)) / (m1 + m2);

    Vec2D<float> v1_normal = rect->GetVelocity() - aligned_vec * v1;
    Vec2D<float> v2_normal = crl->GetVelocity() - aligned_vec * v2;
    rect->SetVelocity(v1_normal + aligned_vec * new_v1);
    crl->SetVelocity(v2_normal + aligned_vec * new_v2);
  }

  void CW(PhysicalCircle* crl, PhysicalWall* wall) {
    Vec2D<float> vec_line = wall->GetA() - wall->GetB();
    Vec2D<float> v = crl->GetVelocity();
    Vec2D<float> v_proc = vec_line * ((vec_line * v) / vec_line.SquaredLength());
    Vec2D<float> v_normal = v - v_proc;
    Vec2D<float> new_v = v_proc - v_normal;
    float factor = (3 * kUniGasConstant * wall->GetTemperature()) / new_v.SquaredLength();
    crl->SetVelocity(new_v * sqrtf(factor));
  }

  void RR(PhysicalRectangle* rect1, PhysicalRectangle* rect2) {
    Vec2D<float> v1 = rect1->GetVelocity();
    Vec2D<float> v2 = rect2->GetVelocity();

    float m1 = rect1->GetMass();
    float m2 = rect2->GetMass();
    Vec2D<float> new_v1 = {(2 * m2 * v2.x + v1.x * (m1 - m2)) / (m1 + m2),
                           (2 * m2 * v2.y + v1.y * (m1 - m2)) / (m1 + m2)};
    Vec2D<float> new_v2 = {(2 * m1 * v1.x + v2.x * (m2 - m1)) / (m1 + m2),
                           (2 * m1 * v1.y + v2.y * (m2 - m1)) / (m1 + m2)};
    rect1->SetVelocity(new_v1);
    rect2->SetVelocity(new_v2);
  }

  void RW(PhysicalRectangle* rect, PhysicalWall* wall) {
    Vec2D<float> vec_line = wall->GetA() - wall->GetB();
    Vec2D<float> v = rect->GetVelocity();
    Vec2D<float> v_proc = vec_line * ((vec_line * v) / vec_line.SquaredLength());
    Vec2D<float> v_normal = v - v_proc;

    Vec2D<float> new_v = v_proc - v_normal;
    float factor = (3 * kUniGasConstant * wall->GetTemperature()) / new_v.SquaredLength();
    rect->SetVelocity(new_v * sqrtf(factor));
  }

  void WW(PhysicalWall* wall1, PhysicalWall* wall2) {}
}

namespace MakeChemistry {
  void CC(MoleculeBox* box, PhysicalCircle* crl1, PhysicalCircle* crl2) {
    float m1 = crl1->GetMass();
    float m2 = crl2->GetMass();
    Vec2D<float> v1 = crl1->GetVelocity();
    Vec2D<float> v2 = crl2->GetVelocity();
    // box->AddShape<PhysicalCircle>((crl1->GetCenter() + crl2->GetCenter()) / (float)2.0,
    //                               (v1 * m1 + v2 * m2) / (m1 + m2),
    //                               m1 + m2,
    //                               sqrtf((m1 + m2) / kPi),
    //                               crl1->GetColor());
    box->AddShape<PhysicalRectangle>((crl1->GetCenter() + crl2->GetCenter()) / 2.0f,
                                     sqrtf(m1 + m2),
                                     sqrtf(m1 + m2),
                                     (v1 * m1 + v2 * m2) / (m1 + m2),
                                     m1 + m2,
                                     kCircleColor);
  }

  void CR(MoleculeBox* box, PhysicalCircle* crl, PhysicalRectangle* rect) {
    float m1 = crl->GetMass();
    float m2 = rect->GetMass();
    Vec2D<float> v1 = crl->GetVelocity();
    Vec2D<float> v2 = rect->GetVelocity();
    box->AddShape<PhysicalRectangle>((crl->GetCenter() + rect->GetCenter()) / 2.0f,
                                      sqrtf(m1 + m2),
                                      sqrtf(m1 + m2),
                                      (v1 * m1 + v2 * m2) / (m1 + m2),
                                      m1 + m2,
                                      kCircleColor);
  }

  void CW(MoleculeBox* box, PhysicalCircle* crl, PhysicalWall* wall) {}

  void RR(MoleculeBox* box, PhysicalRectangle* rect1, PhysicalRectangle* rect2) {
    Vec2D<float> v1 = rect1->GetVelocity();
    Vec2D<float> v2 = rect2->GetVelocity();
    float m1 = rect1->GetMass();
    float m2 = rect2->GetMass();
    float m = m1 + m2;
    Vec2D<float> center = (rect1->GetCenter() + rect2->GetCenter()) / 2.0f;
    Vec2D<float> v = {0.0f, ((v1 * m1 + v2 * m2) / (m1 + m2)).Length()};
    Vec2D<float> center_ofs = {0.0f, 5};
    const size_t num = 6;
    float angle = 2.0f * kPi / (float)num;
    for (size_t i = 0; i < num; ++i) {
      box->AddShape<PhysicalCircle>(Point2D<float>(center + center_ofs),
                                    v, m / num, sqrtf(m / num / kPi), kRectangleColor);
      center_ofs.Rotate(angle);
      v.Rotate(angle);
    }
  }

  void RW(MoleculeBox* box, PhysicalRectangle* rect, PhysicalWall* wall) {}

  void WW(MoleculeBox* box, PhysicalWall* wall1, PhysicalWall* wall2) {}
}


struct ProcessShapeFuncs {
  bool (*DoIntersect)(PhysicalShape* s1, PhysicalShape* s2) = nullptr;
  bool (*IsEnergyEnough)(PhysicalShape* s1, PhysicalShape* s2) = nullptr;
  void (*VelocitysUpdate)(PhysicalShape* s1, PhysicalShape* s2) = nullptr;
  void (*MakeChemistry)(MoleculeBox* box, PhysicalShape* s1, PhysicalShape* s2) = nullptr;
};

void MoleculeBox::ProcessOneIteration(float dt) {
  typedef bool(*Func1)(PhysicalShape* s1, PhysicalShape* s2);
  typedef void (*Func2)(PhysicalShape* s1, PhysicalShape* s2);
  typedef void (*Func3)(MoleculeBox* box, PhysicalShape* s1, PhysicalShape* s2);

  #define PROCESS_SHAPE_FUNCS(TypeType) \
  { (Func1)DoIntersect::TypeType, \
    (Func1)IsEnergyEnough::TypeType, \
    (Func2)VelocitysUpdate::TypeType, \
    (Func3)MakeChemistry::TypeType }

  static ProcessShapeFuncs arr[3][3] = {
    {
      PROCESS_SHAPE_FUNCS(CC),
      PROCESS_SHAPE_FUNCS(CR),
      PROCESS_SHAPE_FUNCS(CW),
    },
    {
      PROCESS_SHAPE_FUNCS(CR),
      PROCESS_SHAPE_FUNCS(RR),
      PROCESS_SHAPE_FUNCS(RW),
    },
    {
      PROCESS_SHAPE_FUNCS(CW),
      PROCESS_SHAPE_FUNCS(RW),
      PROCESS_SHAPE_FUNCS(WW),
    }
  };

	List<PhysicalShape*>::Iterator it1;
	List<PhysicalShape*>::Iterator it2;
  auto shape1 = shapes_.begin();
  while (shape1 != shapes_.end()) {
    auto shape2 = shape1.Next();
  	while (shape2 != shapes_.end()) {
      ShapeType type1 = (*shape1)->type;
      ShapeType type2 = (*shape2)->type;
  		if (type1 < type2) {
  			it1 = shape1;
  			it2 = shape2;
  		} else {
  			it1 = shape2;
  			it2 = shape1;
  		}

      ProcessShapeFuncs funcs = arr[type1][type2];
      if (funcs.DoIntersect(*it1, *it2)) {
        if (funcs.IsEnergyEnough(*it1, *it2)) {
          funcs.MakeChemistry(this, *it1, *it2);
          ++shape1;
          if (shape1 == shape2) {
            ++shape1;
          }
          shapes_.Pop(it1);
          shapes_.Pop(it2);
          break;
        }

        // if they're intersected, update velocitys only if these weren't updated already
        if ((*it1)->collision_partner == nullptr &&
            (*it2)->collision_partner == nullptr) {
          funcs.VelocitysUpdate(*it1, *it2);
          (*it1)->collision_partner = *it2;
        }
      }

      // if they don't intersect and were intersected previously, reset the flag
      else if ((*it1)->collision_partner == *it2) {
        (*it1)->collision_partner = nullptr;
        (*it2)->collision_partner = nullptr;
      }
      ++shape2;
  	}
    ++shape1;
  }

  auto shape = shapes_.begin();
  while (shape != shapes_.end()) {
    (*shape)->Move(dt);
    if (!IsInBound(*shape)) {
      List<PhysicalShape*>::Iterator it = shape++;
      shapes_.Pop(it);
    } else {
      ++shape;
    }
  }
}

void MoleculeBox::Draw(const CoordinateSystem& coord_sys, Render& render) const {
	for (auto shape : shapes_) {
		shape->Draw(coord_sys, render);
	}
}

bool MoleculeBox::IsInBound(PhysicalShape* shape) {
  Point2D<float> center;
  switch (shape->type) {
    case kCircle:
      center = reinterpret_cast<PhysicalCircle*>(shape)->GetCenter();
      return center.x >= left_corner_.x  &&
             center.x <= right_corner_.x &&
             center.y >= right_corner_.y &&
             center.y <= left_corner_.y;
    case kRectangle:
      center = reinterpret_cast<PhysicalRectangle*>(shape)->GetCenter();
      return center.x >= left_corner_.x  &&
             center.x <= right_corner_.x &&
             center.y >= right_corner_.y &&
             center.y <= left_corner_.y;
    case kWall:
      return true;
  }
  return false;
}

void MoleculeBox::IncreaseWallsTemperature(float diff_temp) {
  for (auto shape : shapes_) {
    if (shape->type == kWall) {
      reinterpret_cast<PhysicalWall*>(shape)->IncreaseTemperature(diff_temp);
    }
  }
}

float MoleculeBox::CalculateKineticEnergy() const {
  float res = 0.0f;
  PhysicalCircle* crl = nullptr;
  PhysicalRectangle* rect = nullptr;
  for (auto shape : shapes_) {
    switch (shape->type) {
      case kCircle:
        crl = reinterpret_cast<PhysicalCircle*>(shape);
        res += 0.5f * crl->GetMass() * crl->GetVelocity().SquaredLength();
        break;
      case kRectangle:
        rect = reinterpret_cast<PhysicalRectangle*>(shape);
        res += 0.5f * rect->GetMass() * rect->GetVelocity().SquaredLength();
        break;
      case kWall:
      default:
        break;
    }
  }
  return res;
}