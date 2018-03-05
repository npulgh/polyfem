#include "AssemblerUtils.hpp"

namespace poly_fem
{
	AssemblerUtils &AssemblerUtils::instance()
	{
		static AssemblerUtils instance;

		return instance;
	}


	AssemblerUtils::AssemblerUtils()
	{
		scalar_assemblers_.push_back("Laplacian");

		tensor_assemblers_.push_back("LinearElasticity");
		tensor_assemblers_.push_back("HookeLinearElasticity");
		tensor_assemblers_.push_back("SaintVenant");
	}

	bool AssemblerUtils::is_linear(const std::string &assembler) const
	{
		return assembler != "SaintVenant";
	}

	void AssemblerUtils::assemble_scalar_problem(const std::string &assembler,
		const bool is_volume,
		const int n_basis,
		const std::vector< ElementBases > &bases,
		const std::vector< ElementBases > &gbases,
		Eigen::SparseMatrix<double> &stiffness) const
	{
		// if(assembler == "Laplacian")
		laplacian_.assemble(is_volume, n_basis, bases, gbases, stiffness);
	}

	void AssemblerUtils::assemble_tensor_problem(const std::string &assembler,
		const bool is_volume,
		const int n_basis,
		const std::vector< ElementBases > &bases,
		const std::vector< ElementBases > &gbases,
		Eigen::SparseMatrix<double> &stiffness) const
	{
		if(assembler == "HookeLinearElasticity")
			hooke_linear_elasticity_.assemble(is_volume, n_basis, bases, gbases, stiffness);
		else if(assembler == "SaintVenant")
			return;
		else //if(assembler == "LinearElasticity")
			linear_elasticity_.assemble(is_volume, n_basis, bases, gbases, stiffness);
	}




	double AssemblerUtils::assemble_tensor_energy(const std::string &assembler,
		const bool is_volume,
		const std::vector< ElementBases > &bases,
		const std::vector< ElementBases > &gbases,
		const Eigen::MatrixXd &displacement) const
	{
		if(assembler != "SaintVenant") return 0;

		return saint_venant_elasticity_.compute_energy(is_volume, bases, gbases, displacement);
	}

	void AssemblerUtils::assemble_tensor_energy_gradient(const std::string &assembler,
		const bool is_volume,
		const int n_basis,
		const std::vector< ElementBases > &bases,
		const std::vector< ElementBases > &gbases,
		const Eigen::MatrixXd &displacement,
		Eigen::MatrixXd &grad) const
	{
		if(assembler != "SaintVenant") return;

		saint_venant_elasticity_.assemble(is_volume, n_basis, bases, gbases, displacement, grad);
	}

	void AssemblerUtils::assemble_tensor_energy_hessian(const std::string &assembler,
		const bool is_volume,
		const int n_basis,
		const std::vector< ElementBases > &bases,
		const std::vector< ElementBases > &gbases,
		const Eigen::MatrixXd &displacement,
		Eigen::SparseMatrix<double> &hessian) const
	{
		if(assembler != "SaintVenant") return;

		saint_venant_elasticity_.assemble_grad(is_volume, n_basis, bases, gbases, displacement, hessian);
	}


	void AssemblerUtils::compute_scalar_value(const std::string &assembler,
		const ElementBases &bs,
		const Eigen::MatrixXd &local_pts,
		const Eigen::MatrixXd &fun,
		Eigen::MatrixXd &result) const
	{
		if(assembler == "Laplacian")
			return;
		else if(assembler == "HookeLinearElasticity")
			hooke_linear_elasticity_.local_assembler().compute_von_mises_stresses(bs, local_pts, fun, result);
		else if(assembler == "SaintVenant")
			saint_venant_elasticity_.local_assembler().compute_von_mises_stresses(bs, local_pts, fun, result);
		else //if(assembler == "LinearElasticity")
			linear_elasticity_.local_assembler().compute_von_mises_stresses(bs, local_pts, fun, result);
	}



	void AssemblerUtils::set_parameters(const json &params)
	{
		laplacian_.local_assembler().set_parameters(params);
		linear_elasticity_.local_assembler().set_parameters(params);
		hooke_linear_elasticity_.local_assembler().set_parameters(params);
		saint_venant_elasticity_.local_assembler().set_parameters(params);
	}

}