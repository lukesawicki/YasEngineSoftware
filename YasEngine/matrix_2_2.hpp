#ifndef MATRIX_2_2_HPP
#define MATRIX_2_2_HPP

// element_*_*_ - are named elements or entries wedlug
// wedlug Ilustrowanego slownika matematycznego element

class Matrix_2_2 {
 public:
  float element_0_0_ = 0.0F, element_0_1_ = 0.0F;
  float element_1_0_ = 0.0F, element_1_1_ = 0.0F;

  	static Matrix_2_2* GetTransposedMatrix(const Matrix_2_2* source_matrix) {
    Matrix_2_2* destination_matrix = {
            source_matrix->element_0_0_, source_matrix->element_1_0_,
            source_matrix->element_0_1_, source_matrix->element_1_1_
      };
      return destination_matrix;
    }

    static void TransposeMatrix(Matrix_2_2* matrix) {
      matrix->element_0_0_ = matrix->element_0_0_; matrix->element_0_1_ = matrix->element_1_0_;
      matrix->element_1_0_ = matrix->element_0_1_; matrix->element_1_1_ = matrix->element_1_1_;
    }
};

#endif
