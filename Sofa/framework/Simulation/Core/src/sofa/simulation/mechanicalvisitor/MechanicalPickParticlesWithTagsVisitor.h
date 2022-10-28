/******************************************************************************
*                 SOFA, Simulation Open-Framework Architecture                *
*                    (c) 2006 INRIA, USTL, UJF, CNRS, MGH                     *
*                                                                             *
* This program is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This program is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *
* for more details.                                                           *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this program. If not, see <http://www.gnu.org/licenses/>.        *
*******************************************************************************
* Authors: The SOFA Team and external contributors (see Authors.txt)          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#pragma once

#include <sofa/simulation/BaseMechanicalVisitor.h>
#include <list>

namespace sofa::simulation::mechanicalvisitor
{

/** Find mechanical particles hit by the given ray on dof containing one tag or all provided by a tag list
*
*  A mechanical particle is defined as a 2D or 3D, position or rigid DOF
*  which is linked to the free mechanical DOFs by mechanical mappings
*/
class SOFA_SIMULATION_CORE_API MechanicalPickParticlesWithTagsVisitor : public BaseMechanicalVisitor
{
public:
    type::Vec3d rayOrigin, rayDirection;
    double radius0, dRadius;
    std::list<sofa::core::objectmodel::Tag> tags;
    bool mustContainAllTags;
    typedef std::multimap< double, std::pair<sofa::core::behavior::BaseMechanicalState*, int> > Particles;
    Particles particles;
    MechanicalPickParticlesWithTagsVisitor(const sofa::core::ExecParams* mparams, const type::Vec3d& origin, const type::Vec3d& direction, double r0=0.001, double dr=0.0, std::list<sofa::core::objectmodel::Tag> _tags = std::list<sofa::core::objectmodel::Tag>(), bool _mustContainAllTags = false)
            : BaseMechanicalVisitor(mparams) , rayOrigin(origin), rayDirection(direction), radius0(r0), dRadius(dr), tags(_tags), mustContainAllTags(_mustContainAllTags)
    {
    }

    Result fwdMechanicalState(simulation::Node* /*node*/,sofa::core::behavior::BaseMechanicalState* mm) override;
    Result fwdMechanicalMapping(simulation::Node* /*node*/, sofa::core::BaseMapping* map) override;
    Result fwdMappedMechanicalState(simulation::Node* /*node*/,sofa::core::behavior::BaseMechanicalState* mm) override;

    /// Return a class name for this visitor
    /// Only used for debugging / profiling purposes
    const char* getClassName() const override { return "MechanicalPickParticlesWithTags"; }

#ifdef SOFA_DUMP_VISITOR_INFO
    void setReadWriteVectors() override
	{
	}
#endif

    /// get the closest pickable particle
    void getClosestParticle(sofa::core::behavior::BaseMechanicalState*& mstate, unsigned int& indexCollisionElement, type::Vec3& point, SReal& rayLength );

private:

    // this function checks if the component must be included in the pick process according to its tags
    bool isComponentTagIncluded(const sofa::core::behavior::BaseMechanicalState* mm);

};
}