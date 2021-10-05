#include <math.h>
#include <stdio.h>
#include "../include/MoleculeBox.h"

const float kMinEnergyToReact = 5.0;

MoleculeBox::MoleculeBox(Point2D<float> left_corner,
                         Point2D<float> right_corner,
                         Color frame_color,
                         Color background_color) {
  left_corner_ = left_corner;
  right_corner_ = right_corner;
  frame_color_ = frame_color;
  background_color_ = background_color;

  Point2D<float>& l_c = left_corner_;
  Point2D<float>& r_c = right_corner_;
  Point2D<float> ld_c = {l_c.x, r_c.y};
  Point2D<float> ru_c = {r_c.x, l_c.y};
  // Point2D<float> ld_c = {l_c.x + 3, r_c.y + 3};
  // Point2D<float> ru_c = {r_c.x - 1, l_c.y - 2};
  Color& color = frame_color_;
  typedef PhysicalWall Wall;
  Wall* up_wall    = new Wall(l_c, ru_c, color);
  Wall* left_wall  = new Wall(l_c, ld_c, color);
  Wall* right_wall = new Wall(ru_c, r_c, color);
  Wall* down_wall  = new Wall(ld_c, r_c, color);
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

    Vec2D<float> v1_normal = rect->GetVelocity() - aligned_vec * v1;//
    Vec2D<float> v2_normal = crl->GetVelocity() - aligned_vec * v2;//
    rect->SetVelocity(v1_normal + aligned_vec * new_v1);
    crl->SetVelocity(v2_normal + aligned_vec * new_v2);
  }

  void CW(PhysicalCircle* crl, PhysicalWall* wall) {
    Vec2D<float> vec_line = wall->GetA() - wall->GetB();
    Vec2D<float> velocity = crl->GetVelocity();
    Vec2D<float> velocity_proc = vec_line * ((vec_line * velocity) / vec_line.SquaredLength());
    Vec2D<float> velocity_normal = velocity - velocity_proc;
    crl->SetVelocity(velocity_proc - velocity_normal);
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
    Vec2D<float> velocity = rect->GetVelocity();
    Vec2D<float> velocity_proc = vec_line * ((vec_line * velocity) / vec_line.SquaredLength());
    Vec2D<float> velocity_normal = velocity - velocity_proc;
    rect->SetVelocity(velocity_proc - velocity_normal);
  }

  void WW(PhysicalWall* wall1, PhysicalWall* wall2) {}
}

namespace MakeChemistry {
  void CC(MoleculeBox* box, PhysicalCircle* crl1, PhysicalCircle* crl2) {
    $;
    float m1 = crl1->GetMass();
    float m2 = crl2->GetMass();
    Vec2D<float> v1 = crl1->GetVelocity();
    Vec2D<float> v2 = crl2->GetVelocity();
    // box->AddShape<PhysicalCircle>((crl1->GetCenter() + crl2->GetCenter()) / (float)2.0,
    //                               (v1 * m1 + v2 * m2) / (m1 + m2),
    //                               m1 + m2,
    //                               sqrtf((m1 + m2) / kPi),
    //                               crl1->GetColor());
    box->AddShape<PhysicalRectangle>((crl1->GetCenter() + crl2->GetCenter()) / (float)2.0,
                                     sqrtf(m1 + m2) * 0.5,
                                     sqrtf(m1 + m2) * 0.5,
                                     (v1 * m1 + v2 * m2) / (m1 + m2),
                                     m1 + m2,
                                     crl1->GetColor());
    $$;
  }

  void CR(MoleculeBox* box, PhysicalCircle* crl, PhysicalRectangle* rect) {
    float m1 = crl->GetMass();
    float m2 = rect->GetMass();
    Vec2D<float> v1 = crl->GetVelocity();
    Vec2D<float> v2 = rect->GetVelocity();
    box->AddShape<PhysicalRectangle>((crl->GetCenter() + rect->GetCenter()) / (float)2.0,
                                     sqrtf(m1 + m2),
                                     sqrtf(m1 + m2),
                                     (v1 * m1 + v2 * m2) / (m1 + m2),
                                     m1 + m2,
                                     rect->GetColor());
  }

  void CW(MoleculeBox* box, PhysicalCircle* crl, PhysicalWall* wall) {}

  void RR(MoleculeBox* box, PhysicalRectangle* rect1, PhysicalRectangle* rect2) {
    $;
    Vec2D<float> v1 = rect1->GetVelocity();
    Vec2D<float> v2 = rect2->GetVelocity();
    float m1 = rect1->GetMass();
    float m2 = rect2->GetMass();
    float m = m1 + m2;
    Vec2D<float> center = (rect1->GetCenter() + rect2->GetCenter()) / (float)2.0;
    Vec2D<float> v = {0.0, ((v1 * m1 + v2 * m2) / (m1 + m2)).Length()};
    Vec2D<float> center_ofs = {0.0, 5};
    const size_t num = 6;
    float angle = 2 * kPi / (float)num;
    for (size_t i = 0; i < num; ++i) {
      box->AddShape<PhysicalCircle>(Point2D<float>(center + center_ofs),
                                    v, m / num, sqrtf(m / num / kPi), rect1->GetColor());
      center_ofs.Rotate(angle);
      v.Rotate(angle);
    }
    $$;
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
  $;
  typedef bool(*BoolF)(PhysicalShape* s1, PhysicalShape* s2);
  typedef void (*VoidF)(PhysicalShape* s1, PhysicalShape* s2);
  typedef void (*VoidFF)(MoleculeBox* box, PhysicalShape* s1, PhysicalShape* s2);

  #define PROCESS_SHAPE_FUNCS(TypeType) \
  { (BoolF)DoIntersect::TypeType, \
    (BoolF)IsEnergyEnough::TypeType, \
    (VoidF)VelocitysUpdate::TypeType, \
    (VoidFF)MakeChemistry::TypeType }

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

	List<PhysicalShape*>::iterator it1;
	List<PhysicalShape*>::iterator it2;
  auto elem1 = shapes_.begin();
  while (elem1 != shapes_.end()) {
    $;
    auto elem2 = elem1.Next();
  	while (elem2 != shapes_.end()) {
      $;
      // if (shapes_.GetSize() < 10) {
      //   shapes_.Dump();
      //   printf("node1 = %p\nnode2 = %p\n", elem1.GetNode(), elem2.GetNode());
      // }
  		if ((*elem1)->type < (*elem2)->type) {
  			it1 = elem1;
  			it2 = elem2;
  		} else {
  			it1 = elem2;
  			it2 = elem1;
  		}
      $$;

      ProcessShapeFuncs funcs = arr[(*it1)->type][(*it2)->type];
      // printf("type1 = %d, type2 = %d\n\n\n", (*it1)->type, (*it2)->type);
      if (funcs.DoIntersect(*it1, *it2)) {
        if (funcs.IsEnergyEnough(*it1, *it2)) {
          $;
          funcs.MakeChemistry(this, *it1, *it2);
          ++elem1;
          if (elem1 == elem2) {
            ++elem1;
          }
          shapes_.Pop(it1);
          shapes_.Pop(it2);
          $$;
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
      ++elem2;
  	}
    ++elem1;
    $$;
  }

  auto elem = shapes_.begin();
  while (elem != shapes_.end()) {
    $;
    (*elem)->Move(dt);
    if (!IsInBound(*elem)) {
      List<PhysicalShape*>::iterator it = elem++;
      shapes_.Pop(it);
    } else {
      ++elem;
    }
    $$;
  }
  $$;
}

bool MoleculeBox::IsInBound(PhysicalShape* shape) {
  Point2D<float> center;
  switch (shape->type) {
    case kCircle:
      center = ((PhysicalCircle*)shape)->GetCenter();
      return center.x >= left_corner_.x  &&
             center.x <= right_corner_.x &&
             center.y >= right_corner_.y &&
             center.y <= left_corner_.y;
    case kRectangle:
      center = ((PhysicalRectangle*)shape)->GetCenter();
      return center.x >= left_corner_.x  &&
             center.x <= right_corner_.x &&
             center.y >= right_corner_.y &&
             center.y <= left_corner_.y;
    case kWall:
      return true;
  }
  return false;
}

void MoleculeBox::Draw(CoordinateSystem& coord_sys, Render& render) {
	// Rectangle background(left_corner_, right_corner_, background_color_);
	// background.Draw(coord_sys, render);
  $;
	for (auto elem : shapes_) {
		elem->Draw(coord_sys, render);
	}
  $$;
}