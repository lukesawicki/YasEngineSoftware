#ifndef MATRIX_3_3_HPP
#define MATRIX_3_3_HPP

// element_*_*_ - are named elements or entries wedlug
// wedlug Ilustrowanego slownika matematycznego element

class Matrix_3_3 {
	public:
		float element_0_0_ = 0.0F, element_0_1_ = 0.0F, element_0_2_ = 0.0F;
		float element_1_0_ = 0.0F, element_1_1_ = 0.0F, element_1_2_ = 0.0F;
		float element_2_0_ = 0.0F, element_2_1_ = 0.0F, element_2_2_ = 0.0F;

		static Matrix_3_3* GetTransposedMatrix(const Matrix_3_3* source_matrix)
		{
      Matrix_3_3* destination_matrix = {
            source_matrix->element_0_0_, source_matrix->element_1_0_, source_matrix->element_2_0_,
            source_matrix->element_0_1_, source_matrix->element_1_1_, source_matrix->element_2_1_,
            source_matrix->element_0_2_, source_matrix->element_1_2_, source_matrix->element_2_2_
      };
      return destination_matrix;
    }

    static void TransposeMatrix(Matrix_3_3* matrix) {
      matrix->element_0_0_ = matrix->element_0_0_; matrix->element_0_1_ = matrix->element_1_0_; matrix->element_0_2_ = matrix->element_2_0_;
      matrix->element_1_0_ = matrix->element_0_1_; matrix->element_1_1_ = matrix->element_1_1_; matrix->element_1_2_ = matrix->element_2_1_;
      matrix->element_2_0_ = matrix->element_0_2_; matrix->element_2_1_ = matrix->element_1_2_; matrix->element_2_2_ = matrix->element_2_2_;

    }
};

#endif
