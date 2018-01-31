#ifndef ZERO_BC_PROBLEM_HPP
#define ZERO_BC_PROBLEM_HPP

#include "Problem.hpp"

#include <vector>
#include <Eigen/Dense>

namespace poly_fem
{
	class ZeroBCProblem: public Problem
	{
	public:
		ZeroBCProblem();

		void rhs(const Mesh &mesh, const Eigen::MatrixXd &pts, Eigen::MatrixXd &val) const override;
		void bc(const Mesh &mesh, const Eigen::MatrixXi &global_ids, const Eigen::MatrixXd &pts, Eigen::MatrixXd &val) const override;

		void exact(const Eigen::MatrixXd &pts, Eigen::MatrixXd &val) const override;

		bool has_exact_sol() const override { return true; }
		bool is_scalar() const override { return true; }
	};
}

#endif //ZERO_BC_PROBLEM_HPP

