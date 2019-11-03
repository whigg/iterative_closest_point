#include "pca.h"
#include <iostream>
#include <ostream>

void k_nearest_neighbour(const MatrixXd &V1,Eigen::MatrixXi &I, int k){
  // need to get k nearest_neighbour indexes
  // it will be in variable I
  std::vector<std::vector<int > > O_PI;
  Eigen::MatrixXi O_CH;
  Eigen::MatrixXd O_CN;
  Eigen::VectorXd O_W;

  igl::octree(V1,O_PI,O_CH,O_CN,O_W);
  igl::knn(V1, k, O_PI, O_CH,O_CN, O_W, I);
}



void compute_normals(const MatrixXd &V1,const Eigen::MatrixXi &I, int k, MatrixXd &normals){
    // compute the normals using PCA
    for (int j=0; j < V1.rows(); j++){
      // local variable for neighbours coordinates
      MatrixXd neighb_points = MatrixXd::Zero(3,k);
      // points in neighbourhood
      for (int i = 0; i < k; i++){
        neighb_points.col(i) = V1.transpose().col(I(j,i));
      }
      //find a mean for neighb_points (centroid)
      VectorXd m = neighb_points.rowwise().mean();

      //substruct mean from all values
      MatrixXd delta_m = neighb_points;
      delta_m.colwise() -= m;

      //scatter matrix S
      MatrixXd S = delta_m * delta_m.transpose();

      //SVD
      JacobiSVD<MatrixXd> svd(S, ComputeThinU | ComputeThinV);

      //Getting plane normal (the last column in U)
      int last_index = svd.matrixU().cols();
      normals.row(j) = svd.matrixU().transpose().row(last_index - 1);
    }
  }
