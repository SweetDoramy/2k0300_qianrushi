#include"imu.h"
#include"math.h"

 
/***********************************************矩阵区域***********************************************************/
 // 获取数组中元素的索引
 int get_index(int row, int col, int num_cols) {
     return row * num_cols + col;
 }
 
 // 矩阵乘法
 void matrix_multiply(float* A, float* B, float* C, int rows_A, int cols_A, int cols_B) {
    if (A == NULL || B == NULL || C == NULL) {
        // 处理空指针错误
        return;
    }

     for (int i = 0; i < rows_A; i++) {
         for (int j = 0; j < cols_B; j++) {
             float sum = 0.0;
             for (int k = 0; k < cols_A; k++) {
                 sum += A[get_index(i, k, cols_A)] * B[get_index(k, j, cols_B)];
             }
             C[get_index(i, j, cols_B)] = sum;
         }
     }
 }
 
 // 矩阵加法
 void matrix_add(float* A, float* B, float* C, int rows, int cols) {
    if (A == NULL || B == NULL || C == NULL) {
        // 处理空指针错误
        return;
    }

     for (int i = 0; i < rows; i++) {
         for (int j = 0; j < cols; j++) {
             C[get_index(i, j, cols)] = A[get_index(i, j, cols)] + B[get_index(i, j, cols)];
         }
     }
 }
 
 // 矩阵减法
 void matrix_subtract(float* A, float* B, float* C, int rows, int cols) {
    if (A == NULL || B == NULL || C == NULL) {
        // 处理空指针错误
        return;
    }

     for (int i = 0; i < rows; i++) {
         for (int j = 0; j < cols; j++) {
             C[get_index(i, j, cols)] = A[get_index(i, j, cols)] - B[get_index(i, j, cols)];
         }
     }
 }
 
 // 矩阵转置
 void matrix_transpose(float* A, float* B, int rows, int cols) {
    if (A == NULL || B == NULL) {
        // 处理空指针错误
        return;
    }

     for (int i = 0; i < rows; i++) {
         for (int j = 0; j < cols; j++) {
             B[get_index(j, i, rows)] = A[get_index(i, j, cols)];
         }
     }
 }
 
 // 矩阵复制
 void matrix_copy(float* A, float* B, int rows, int cols) {
    if (A == NULL || B == NULL) {
        // 处理空指针错误
        return;
    }

     for (int i = 0; i < rows; i++) {
         for (int j = 0; j < cols; j++) {
             B[get_index(i, j, cols)] = A[get_index(i, j, cols)];
         }
     }
 }
 
 // 计算矩阵的行列式
 float calculateDeterminant(float* matrix) {
    if (matrix == NULL) {
        // 处理空指针错误
        return 0.0f;
    }

     float a = matrix[0];
     float b = matrix[1];
     float c = matrix[2];
     float d = matrix[3];
     float e = matrix[4];
     float f = matrix[5];
     float g = matrix[6];
     float h = matrix[7];
     float i = matrix[8];
 
     return (a * e * i) + (b * f * g) + (c * d * h) - (c * e * g) - (b * d * i) - (a * f * h);
 }
 
 // 计算矩阵的伴随矩阵
 void calculateAdjoint(float* matrix, float* adjoint) {
    if (matrix == NULL || adjoint == NULL) {
        // 处理空指针错误
        return;
    }

     float a = matrix[0];
     float b = matrix[1];
     float c = matrix[2];
     float d = matrix[3];
     float e = matrix[4];
     float f = matrix[5];
     float g = matrix[6];
     float h = matrix[7];
     float i = matrix[8];
 
     adjoint[0] = (e * i) - (f * h);
     adjoint[1] = (c * h) - (b * i);
     adjoint[2] = (b * f) - (c * e);
     adjoint[3] = (f * g) - (d * i);
     adjoint[4] = (a * i) - (c * g);
     adjoint[5] = (c * d) - (a * f);
     adjoint[6] = (d * h) - (e * g);
     adjoint[7] = (g * b) - (a * h);
     adjoint[8] = (a * e) - (b * d);
 }
 
 // 计算矩阵的逆矩阵
 int calculateInverse(float* matrix, float* inverse) {
    if (matrix == NULL || inverse == NULL) {
        // 处理空指针错误
        return 0;
    }

     float determinant = calculateDeterminant(matrix);
     if (determinant == 0) {
         return 0;  // 逆矩阵不存在
     }
 
     float adjoint[9];
     calculateAdjoint(matrix, adjoint);
 
     // 逆矩阵 = 伴随矩阵 / 行列式
     for (int i = 0; i < 9; i++) {
         inverse[i] = adjoint[i] / determinant;
     }
 
     return 1;
 }
 
 
 /**
   * @brief  LU分解
   * @param  None
   * @note   None
   * @retval None
   */
 void luDecompose(float *A, float *L, float *U, int n) {
     for (int i = 0; i < n; i++) {
         // 计算U的第一行
         for (int j = i; j < n; j++) {
             U[i * n + j] = A[i * n + j];
             for (int k = 0; k < i; k++) {
                 U[i * n + j] -= L[i * n + k] * U[k * n + j];
             }
         }
 
         // 计算L的第一列
         for (int j = i; j < n; j++) {
             if (i == j) {
                 L[i * n + j] = 1.0;
             } else {
                 L[j * n + i] = A[j * n + i];
                 for (int k = 0; k < i; k++) {
                     L[j * n + i] -= L[j * n + k] * U[k * n + i];
                 }
                 L[j * n + i] /= U[i * n + i];
             }
         }
     }
 }


/***********************************************矩阵区域***********************************************************/

ImuStructTypeDef ImuStruct;
static MahonyStructTypeDef ImuMahonyStruct;   // Mahony
static EKFStructTypeDef ImuEkfStruct;         // ekf 

static int16 GYRO_X_SHIFT = 0;
static int16 GYRO_Y_SHIFT = 0;
static int16 GYRO_Z_SHIFT = 0;

static uint8 counter_yaw = 0;            //为了获取偏航角积分时的初始值设置的计数

/***********************************************转换方法***********************************************************/
/**
  * @brief  Quaternion归一化
  * @param  None
  * @note   None
  * @retval None
  */
 static void normalize_quternion(float* qt)
 {
     float norm = sqrtf(qt[0] * qt[0] + qt[1] * qt[1] + qt[2] * qt[2] + qt[3] * qt[3]);
     qt[0] /= norm;
     qt[1] /= norm;
     qt[2] /= norm;
     qt[3] /= norm;
 }
 
 /**
   * @brief  四元数转换欧拉角
   * @param  None
   * @note   None
   * @retval None
   */
 static void quternion2angle(const float* qt, float* pitch, float* roll)
 {
     *roll  = atan2f(2.0f * (qt[2] * qt[3] + qt[0] * qt[1]),2.0f * (qt[0] * qt[0] + qt[3] * qt[3]) - 1.0f) * DIV_180_PI;
     *pitch = asinf(-2.0f * (qt[1] * qt[3] - qt[0] * qt[2])) * DIV_180_PI;
 }


/**
  * @brief  四元数转换偏航角(弃用)
  * @param  None
  * @note   None
  * @retval None
  */
 static void quternion2yaw(const float* qt, float* yaw)
 {
     *yaw = atan2f(2.0f * (qt[0] * qt[3] + qt[2] * qt[1]),2.0f * (qt[0] * qt[0] + qt[1] * qt[1]) - 1.0f) * DIV_180_PI;
 }
 


/**
  * @brief  扩展卡尔曼 优化四元数
  * @param  gx      x轴角速度
  * @param  gy      y轴角速度
  * @param  gz      z轴角速度
  * @param  ax      x轴加速度
  * @param  ay      y轴加速度
  * @param  az      z轴加速度
  * @note   None
  * @retval None
  */
 static void ekf_update(float gx, float gy, float gz, float ax, float ay, float az)
 {
     gx /= DIV_180_PI;
     gy /= DIV_180_PI;
     gz /= DIV_180_PI;
 
     ax *= GRAVITY;
     ay *= GRAVITY;
     az *= GRAVITY;
 
     normalize_quternion(ImuEkfStruct.q);
     /****************************** 状态方程 ******************************/
     /* 1/2姿态更新周期赋值 */
     float half_T = ImuEkfStruct.half_T;
     /* 四元数微分方程离散化 */
     gx *= half_T;
     gy *= half_T;
     gz *= half_T;
     ImuEkfStruct.q[0] += (-ImuEkfStruct.q_k[1] * gx - ImuEkfStruct.q_k[2] * gy - ImuEkfStruct.q_k[3] * gz);
     ImuEkfStruct.q[1] += ( ImuEkfStruct.q_k[0] * gx + ImuEkfStruct.q_k[2] * gz - ImuEkfStruct.q_k[3] * gy);
     ImuEkfStruct.q[2] += ( ImuEkfStruct.q_k[0] * gy - ImuEkfStruct.q_k[1] * gz + ImuEkfStruct.q_k[3] * gx);
     ImuEkfStruct.q[3] += ( ImuEkfStruct.q_k[0] * gz + ImuEkfStruct.q_k[1] * gy - ImuEkfStruct.q_k[2] * gx);
     /*   四元数 归一化    */
     normalize_quternion(ImuEkfStruct.q);
     /*  状态转移矩阵赋值   */
     ImuEkfStruct.A[0] =            1;  ImuEkfStruct.A[1] = -gx * half_T; ImuEkfStruct.A[2] = -gy * half_T;  ImuEkfStruct.A[3] = -gz * half_T;
     ImuEkfStruct.A[4] =  gx * half_T;  ImuEkfStruct.A[5] =            1; ImuEkfStruct.A[6] =  gz * half_T;  ImuEkfStruct.A[7] = -gy * half_T;
     ImuEkfStruct.A[8] =  gy * half_T;  ImuEkfStruct.A[9] = -gz * half_T; ImuEkfStruct.A[10]=            1;  ImuEkfStruct.A[11]=  gx * half_T;
     ImuEkfStruct.A[12]=  gz * half_T;  ImuEkfStruct.A[13]=  gy * half_T; ImuEkfStruct.A[14]= -gx * half_T;  ImuEkfStruct.A[15]=            1;
     /****************************** 状态方程 ******************************/
 
     /**************************** 估算协方差矩阵 ***************************/
     float temp_matrix1[16];
     float temp_matrix2[16];
     float temp_matrix3[16];
     matrix_multiply(ImuEkfStruct.A,ImuEkfStruct.p_hat,temp_matrix1,4,4,4);
     matrix_transpose(ImuEkfStruct.A,temp_matrix2,4,4);
     matrix_multiply(temp_matrix1,temp_matrix2,temp_matrix3,4,4,4);
     matrix_add(temp_matrix3,ImuEkfStruct.Q_matrix,ImuEkfStruct.p_hat,4,4);
     /**************************** 估算协方差矩阵 ***************************/
 
     /****************************** 观测方程 ******************************/
     /*   观测向量计算  */
     ImuEkfStruct.vector_hat[0] = 2 * (ImuEkfStruct.q[1] * ImuEkfStruct.q[3] - ImuEkfStruct.q[0] * ImuEkfStruct.q[2]);
     ImuEkfStruct.vector_hat[1] = 2 * (ImuEkfStruct.q[2] * ImuEkfStruct.q[3] + ImuEkfStruct.q[0] * ImuEkfStruct.q[1]);
     ImuEkfStruct.vector_hat[2] = 1.0f - 2 * (ImuEkfStruct.q[1] * ImuEkfStruct.q[1]) - 2 * (ImuEkfStruct.q[2] * ImuEkfStruct.q[2]);
     /*    观测矩阵赋值    */
     ImuEkfStruct.H[0] = -2 * ImuEkfStruct.q[2]; ImuEkfStruct.H[1] =  2 * ImuEkfStruct.q[3]; ImuEkfStruct.H[2] =  -2 * ImuEkfStruct.q[0]; ImuEkfStruct.H[3] =  2 * ImuEkfStruct.q[1];
     ImuEkfStruct.H[4] =  2 * ImuEkfStruct.q[1]; ImuEkfStruct.H[5] =  2 * ImuEkfStruct.q[0]; ImuEkfStruct.H[6] =   2 * ImuEkfStruct.q[3]; ImuEkfStruct.H[7] =  2 * ImuEkfStruct.q[2];
     ImuEkfStruct.H[8] =  2 * ImuEkfStruct.q[0]; ImuEkfStruct.H[9] = -2 * ImuEkfStruct.q[1]; ImuEkfStruct.H[10] = -2 * ImuEkfStruct.q[2]; ImuEkfStruct.H[11] = 2 * ImuEkfStruct.q[3];
     /****************************** 观测方程 ******************************/
 
     /**************************** 计算卡尔曼增益 ***************************/
     float temp_matrix4[12];
     float temp_matrix5[12];
     float temp_matrix6[9];
     float temp_matrix7[9];
     float temp_matrix8[9];
     float HT[12];
     float L[9];
     float U[9];
     float LInv[9];
     float UInv[9];
     matrix_transpose(ImuEkfStruct.H,HT,3,4);
     matrix_multiply(ImuEkfStruct.p_hat,HT,temp_matrix4,4,4,3);
     matrix_multiply(ImuEkfStruct.H,ImuEkfStruct.p_hat,temp_matrix5,3,4,4);
     matrix_multiply(temp_matrix5,HT,temp_matrix6,3,4,3);
     matrix_add(temp_matrix6,ImuEkfStruct.R_matrix,temp_matrix7,3,3);
     /*  LU分解 */
     luDecompose(temp_matrix7,L,U,3);
     /*  三角阵求逆重组  */
     calculateInverse(L,LInv);
     calculateInverse(U,UInv);
     matrix_multiply(UInv,LInv,temp_matrix8,3,3,3);
     matrix_multiply(temp_matrix4,temp_matrix8,ImuEkfStruct.K,4,3,3);
     /**************************** 计算卡尔曼增益 ***************************/
 
     /****************************** 计算残差 ******************************/
     /*   归一化加速度计    */
     float norm = sqrtf(ax * ax + ay * ay + az* az);
     ax /= norm;
     ay /= norm;
     az /= norm;
     /*    计算残差向量    */
     ImuEkfStruct.T[0] = ax - ImuEkfStruct.vector_hat[0];
     ImuEkfStruct.T[1] = ay - ImuEkfStruct.vector_hat[1];
     ImuEkfStruct.T[2] = az - ImuEkfStruct.vector_hat[2];
     /****************************** 计算残差 ******************************/
 
     /*************************** 后验估计四元数 ****************************/
     matrix_multiply(ImuEkfStruct.K,ImuEkfStruct.T,ImuEkfStruct.Kq,4,3,1);
     /*    后验四元数      */
     ImuEkfStruct.q_k[0] = ImuEkfStruct.q[0] + ImuEkfStruct.Kq[0];
     ImuEkfStruct.q_k[1] = ImuEkfStruct.q[1] + ImuEkfStruct.Kq[1];
     ImuEkfStruct.q_k[2] = ImuEkfStruct.q[2] + ImuEkfStruct.Kq[2];
     ImuEkfStruct.q_k[3] = ImuEkfStruct.q[3] + ImuEkfStruct.Kq[3];
     /*    归一化四元数     */
     normalize_quternion(ImuEkfStruct.q_k);
     /*    更新 四元数     */
     ImuEkfStruct.q[0] = ImuEkfStruct.q_k[0];
     ImuEkfStruct.q[1] = ImuEkfStruct.q_k[1];
     ImuEkfStruct.q[2] = ImuEkfStruct.q_k[2];
     ImuEkfStruct.q[3] = ImuEkfStruct.q_k[3];
     /*************************** 后验估计四元数 ****************************/
 
     /*************************** 更新协方差矩阵 ****************************/
     float temp_matrix9[16];
     float temp_matrix10[16];
     float temp_matrix11[16];
     matrix_multiply(ImuEkfStruct.K,ImuEkfStruct.H,temp_matrix9,4,3,4);
     matrix_subtract(ImuEkfStruct.I_matrix,temp_matrix9,temp_matrix10,4,4);
     matrix_multiply(temp_matrix10,ImuEkfStruct.p_hat,temp_matrix11,4,4,4);
     matrix_copy(temp_matrix11,ImuEkfStruct.p_hat,4,4);
     /*************************** 更新协方差矩阵 ****************************/
 }

 /**
  * @brief  EKF初始化
  * @param  None
  * @note   None
  * @retval None
  */
static void EKF_init(void)
{
    ImuEkfStruct.period = 0.001;
    ImuEkfStruct.half_T = ImuEkfStruct.period / 2.0f;

    ImuEkfStruct.I_matrix[0] = 1; ImuEkfStruct.I_matrix[1] = 0;   ImuEkfStruct.I_matrix[2] = 0;  ImuEkfStruct.I_matrix[3] = 0;
    ImuEkfStruct.I_matrix[4] = 0; ImuEkfStruct.I_matrix[5] = 1;   ImuEkfStruct.I_matrix[6] = 0;  ImuEkfStruct.I_matrix[7] = 0;
    ImuEkfStruct.I_matrix[8] = 0; ImuEkfStruct.I_matrix[9] = 0;   ImuEkfStruct.I_matrix[10] = 1; ImuEkfStruct.I_matrix[11] = 0;
    ImuEkfStruct.I_matrix[12] = 0;ImuEkfStruct.I_matrix[13] = 0;  ImuEkfStruct.I_matrix[14] = 0; ImuEkfStruct.I_matrix[15] = 1;

    float Q_Val = 0.01;
    ImuEkfStruct.Q_matrix[0] = Q_Val; ImuEkfStruct.Q_matrix[1] = 0;     ImuEkfStruct.Q_matrix[2] = 0;     ImuEkfStruct.Q_matrix[3] = 0;
    ImuEkfStruct.Q_matrix[4] = 0;     ImuEkfStruct.Q_matrix[5] = Q_Val; ImuEkfStruct.Q_matrix[6] = 0;     ImuEkfStruct.Q_matrix[7] = 0;
    ImuEkfStruct.Q_matrix[8] = 0;     ImuEkfStruct.Q_matrix[9] = 0;     ImuEkfStruct.Q_matrix[10] = Q_Val;ImuEkfStruct.Q_matrix[11] = 0;
    ImuEkfStruct.Q_matrix[12] = 0;    ImuEkfStruct.Q_matrix[13] = 0;    ImuEkfStruct.Q_matrix[14] = 0;    ImuEkfStruct.Q_matrix[15] = Q_Val;

    float R_Val = 1000000;
    ImuEkfStruct.R_matrix[0] = R_Val; ImuEkfStruct.R_matrix[1] = 0;     ImuEkfStruct.R_matrix[2] = 0;
    ImuEkfStruct.R_matrix[3] = 0;     ImuEkfStruct.R_matrix[4] = R_Val; ImuEkfStruct.R_matrix[5] = 0;
    ImuEkfStruct.R_matrix[6] = 0;     ImuEkfStruct.R_matrix[7] = 0;     ImuEkfStruct.R_matrix[8] = R_Val;

    float P_Val = 100000;
    ImuEkfStruct.p_hat[0] = P_Val;ImuEkfStruct.p_hat[1] = 0;    ImuEkfStruct.p_hat[2] = 0;     ImuEkfStruct.p_hat[3] = 0;
    ImuEkfStruct.p_hat[4] = 0;    ImuEkfStruct.p_hat[5] = P_Val;ImuEkfStruct.p_hat[6] = 0;     ImuEkfStruct.p_hat[7] = 0;
    ImuEkfStruct.p_hat[8] = 0;    ImuEkfStruct.p_hat[9] = 0;    ImuEkfStruct.p_hat[10] = P_Val;ImuEkfStruct.p_hat[11] = 0;
    ImuEkfStruct.p_hat[12] = 0;   ImuEkfStruct.p_hat[13] = 0;   ImuEkfStruct.p_hat[14] = 0;    ImuEkfStruct.p_hat[15] = P_Val;

    ImuEkfStruct.q[0] = 1;
    ImuEkfStruct.q[1] = 0;
    ImuEkfStruct.q[2] = 0;
    ImuEkfStruct.q[3] = 0;

    ImuEkfStruct.q_k[0] = 1;
    ImuEkfStruct.q_k[1] = 0;
    ImuEkfStruct.q_k[2] = 0;
    ImuEkfStruct.q_k[3] = 0;

    ImuEkfStruct.filter = ekf_update;

    ImuStruct.yaw = 0;
}



/**
  * @brief  Mahony姿态更新
  * @param  gx      x轴角速度
  * @param  gy      y轴角速度
  * @param  gz      z轴角速度
  * @param  ax      x轴加速度
  * @param  ay      y轴加速度
  * @param  az      z轴加速度
  * @note   None
  * @retval None
  */
 static void MahonyAHRSupdateIMU (float gx, float gy, float gz, float ax, float ay, float az)
 {
 
     gx /= DIV_180_PI;
     gy /= DIV_180_PI;
     gz /= DIV_180_PI;
 
     ax *= GRAVITY;
     ay *= GRAVITY;
     az *= GRAVITY;
 
     float halfvx, halfvy, halfvz;
     float halfex, halfey, halfez;
     float qa, qb, qc;
 
     static float half_error;
     static int count = 0;
     /********************************参数规划*********************************************************************************************************************/
 
     if (count < 3000)
     {
         count += 2;
         ImuMahonyStruct.twoKp = 20;
     }
     else
     {
         if (fabs(half_error * 1000) > 50)
             ImuMahonyStruct.twoKp = 0.1;
         else
             ImuMahonyStruct.twoKp = 4;
     }
 
     /*****************************************************************************************************************************************************/
 
     // Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
     // 仅当加速度计测量有效时才计算反馈（避免在加速度计归一化中使用 NaN）
     if (!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f)))
     {
 
         // Normalise accelerometer measurement
         // 标准化加速度计测量
         normalize_quternion(ImuMahonyStruct.q);
 
         // Estimated direction of gravity and vector perpendicular to magnetic flux
         // 垂直于磁通量的重力和矢量的估计方向     通过四元数计算预测的重力方向
         halfvx = ImuMahonyStruct.q[1] * ImuMahonyStruct.q[3] - ImuMahonyStruct.q[0] * ImuMahonyStruct.q[2];
         halfvy = ImuMahonyStruct.q[0] * ImuMahonyStruct.q[1] + ImuMahonyStruct.q[2] * ImuMahonyStruct.q[3];
         halfvz = ImuMahonyStruct.q[0] * ImuMahonyStruct.q[0] - 0.5f + ImuMahonyStruct.q[3] * ImuMahonyStruct.q[3];
 
         // Error is sum of cross product between estimated and measured direction of gravity
         // 误差是估计的重力方向和测量的重力方向之间的叉积之和   将预测的重力方向与加速度计测得的重力方向做叉积，得到误差向量
         halfex = (ay * halfvz - az * halfvy);
         halfey = (az * halfvx - ax * halfvz);
         halfez = (ax * halfvy - ay * halfvx);
 
         half_error = sqrtf(halfex * halfex + halfey * halfey + halfez * halfez);
 
         // Compute and apply integral feedback if enabled
         // 计算并应用积分反馈（如果启用）
         if (ImuMahonyStruct.twoKi > 0.0f)
         {
             ImuMahonyStruct.integralFBx += ImuMahonyStruct.twoKi * halfex * (1.0f / ImuMahonyStruct.sampleFreq);    // integral error scaled by Ki
             ImuMahonyStruct.integralFBy += ImuMahonyStruct.twoKi * halfey * (1.0f / ImuMahonyStruct.sampleFreq);
             ImuMahonyStruct.integralFBz += ImuMahonyStruct.twoKi * halfez * (1.0f / ImuMahonyStruct.sampleFreq);
             gx += ImuMahonyStruct.integralFBx;  // apply integral feedback
             gy += ImuMahonyStruct.integralFBy;
             gz += ImuMahonyStruct.integralFBz;
         }
         else
         {
             ImuMahonyStruct.integralFBx = 0.0f; // prevent integral windup
             ImuMahonyStruct.integralFBy = 0.0f;
             ImuMahonyStruct.integralFBz = 0.0f;
         }
 
         // Apply proportional feedback
         gx += ImuMahonyStruct.twoKp * halfex;
         gy += ImuMahonyStruct.twoKp * halfey;
         gz += ImuMahonyStruct.twoKp * halfez;
     }
 
     // Integrate rate of change of quaternion
     // 四元数的积分变化率
     gx *= (0.5f * (1.0f / ImuMahonyStruct.sampleFreq));     // pre-multiply common factors  预乘公因数
     gy *= (0.5f * (1.0f / ImuMahonyStruct.sampleFreq));
     gz *= (0.5f * (1.0f / ImuMahonyStruct.sampleFreq));
     qa = ImuMahonyStruct.q[0];
     qb = ImuMahonyStruct.q[1];
     qc = ImuMahonyStruct.q[2];
     ImuMahonyStruct.q[0] += (-qb * gx - qc * gy - ImuMahonyStruct.q[3] * gz);
     ImuMahonyStruct.q[1] += (qa * gx + qc * gz - ImuMahonyStruct.q[3] * gy);
     ImuMahonyStruct.q[2] += (qa * gy - qb * gz + ImuMahonyStruct.q[3] * gx);
     ImuMahonyStruct.q[3] += (qa * gz + qb * gy - qc * gx);
 
     // Normalise quaternion
     normalize_quternion(ImuMahonyStruct.q);
 
 }
 
 /**
   * @brief  Mahony初始化
   * @param  None
   * @note   四元数的初始化和归一化
   * @retval None
   */
 static void Mahony_init(void)
 {
    // 初始化参数
     ImuMahonyStruct.twoKp = 2.0f * 0.1f;       // 比例反馈增益
     ImuMahonyStruct.twoKi = 2.0f * 0.0f;       // 积分反馈增益
 
     // 初始化积分反馈为零
     ImuMahonyStruct.integralFBx = 0;
     ImuMahonyStruct.integralFBy = 0;
     ImuMahonyStruct.integralFBz = 0;
 
     // 初始化四元数为单位四元数
     ImuMahonyStruct.q[0] = 1;
     ImuMahonyStruct.q[1] = 0;
     ImuMahonyStruct.q[2] = 0;
     ImuMahonyStruct.q[3] = 0;
 
     // 设置采样频率（Hz）
     ImuMahonyStruct.sampleFreq = 1000.0f;         //周期是1ms
 
     // 设置滤波器更新函数
     ImuMahonyStruct.filter = MahonyAHRSupdateIMU;
 
 }


/**
  * @brief  imu更新姿态
  * @param  roll        // 翻滚角返回指针
  * @param  pitch       // 俯仰角返回指针
  * @param  mode        // 优化方式
  * @note   None
  * @retval None
  */
 void imu_update(void)
{
    float p,r,y,t;
    float dt = 1e-3;

    float acc_trans =  4098;        //加速度量程为:±8G,除以4098      
    float gyro_trans =  14.3;          //陀螺仪量程为:±2000dps，除以14.3
    float mag_trans =  3000;        //磁力计量程为:±8G,数据除以 3000

    //获取原始物理值，这一步必须先在定时器中实现，不然之后没有获取值
    if(DEV_IMU660RA == imu_type || DEV_IMU660RB == imu_type)
    {
        imu_acc_x = imu_get_raw(imu_file_path[ACC_X_RAW]);
        imu_acc_y = imu_get_raw(imu_file_path[ACC_Y_RAW]);
        imu_acc_z = imu_get_raw(imu_file_path[ACC_Z_RAW]);
    
        imu_gyro_x = imu_get_raw(imu_file_path[GYRO_X_RAW]);
        imu_gyro_y = imu_get_raw(imu_file_path[GYRO_Y_RAW]);
        imu_gyro_z = imu_get_raw(imu_file_path[GYRO_Z_RAW]);
    }
    else if(DEV_IMU963RA == imu_type)
    {
        imu_acc_x = imu_get_raw(imu_file_path[ACC_X_RAW]);
        imu_acc_y = imu_get_raw(imu_file_path[ACC_Y_RAW]);
        imu_acc_z = imu_get_raw(imu_file_path[ACC_Z_RAW]);
    
        imu_gyro_x = imu_get_raw(imu_file_path[GYRO_X_RAW]);
        imu_gyro_y = imu_get_raw(imu_file_path[GYRO_Y_RAW]);
        imu_gyro_z = imu_get_raw(imu_file_path[GYRO_Z_RAW]);

        imu_mag_x = imu_get_raw(imu_file_path[MAG_X_RAW]);
        imu_mag_y = imu_get_raw(imu_file_path[MAG_Y_RAW]);
        imu_mag_z = imu_get_raw(imu_file_path[MAG_Z_RAW]);
    }

    //转化成实际物理量值
    ImuStruct.accX = (float)imu_acc_x / acc_trans;
    ImuStruct.accY = (float)imu_acc_y / acc_trans;
    ImuStruct.accZ = (float)imu_acc_z / acc_trans;

    ImuStruct.gyroX = (float)imu_gyro_x / gyro_trans - (float)GYRO_X_SHIFT / gyro_trans;
    ImuStruct.gyroY = (float)imu_gyro_y / gyro_trans - (float)GYRO_Y_SHIFT / gyro_trans;
    ImuStruct.gyroZ = (float)imu_gyro_z / gyro_trans - (float)GYRO_Z_SHIFT / gyro_trans;

    ImuStruct.magX = (float)imu_mag_x / mag_trans;
    ImuStruct.magY = (float)imu_mag_y / mag_trans;
    ImuStruct.magZ = (float)imu_mag_z / mag_trans;



#if !IMU_MODE
    ImuEkfStruct.filter(ImuStruct.gyroX, ImuStruct.gyroY, ImuStruct.gyroZ, ImuStruct.accX, ImuStruct.accY, ImuStruct.accZ);
    quternion2angle(ImuEkfStruct.q, &p, &r);
    quternion2yaw(ImuMahonyStruct.q, &y);

#else
    ImuMahonyStruct.filter(ImuStruct.gyroX, ImuStruct.gyroY, ImuStruct.gyroZ, ImuStruct.accX, ImuStruct.accY, ImuStruct.accZ);
    quternion2angle(ImuMahonyStruct.q, &p, &r);
    quternion2yaw(ImuMahonyStruct.q, &y);

#endif

    ImuStruct.pitch = p;
    ImuStruct.roll = r;
    ImuStruct.yaw = y*360/17.7 + 360;
    if(0){
        while(ImuStruct.yaw>360){
            ImuStruct.yaw -= 360;
        } 
        while(ImuStruct.yaw < 0) {
            ImuStruct.yaw += 360.0;
        }
    }


    //偏航角积分
    integral_yaw_run();      
    // printf("yaw = %f.\r\n",ImuStruct.yaw);
}



/**
  * @brief  imu初始化
  * @param  None
  * @note   系统启动或静止期间进行校准，消除零漂，只在初始化部分卡了2s
  * @retval None
  */
 void imu_init(void)
 {
    imu_get_dev_info();            //先获取设备信息

     /************ 消零漂 **************/
     int32 x_shift_temp = 0;
     int32 y_shift_temp = 0;
     int32 z_shift_temp = 0;
     uint8 shift_num = 0;
 
     while(shift_num != 200){
        // imu963ra_get_gyro();
         x_shift_temp += (int32)imu_gyro_x;
         y_shift_temp += (int32)imu_gyro_y;
         z_shift_temp += (int32)imu_gyro_z;
         shift_num ++;
         system_delay_ms(10);
     }
 
     GYRO_X_SHIFT = (int16)(x_shift_temp / (int32)shift_num);
     GYRO_Y_SHIFT = (int16)(y_shift_temp / (int32)shift_num);
     GYRO_Z_SHIFT = (int16)(z_shift_temp / (int32)shift_num);
     /************ 消零漂 **************/
     ImuStruct.yaw = 0;
 
 #if !IMU_MODE
     EKF_init();
 #else
     Mahony_init();
 #endif
    // !!!! 初始化一定要延时，不然会发散 !!!!
    system_delay_ms(100);
    beep_run();

    ImuStruct.integral_yaw_flag = 0;                     //初始偏航角标志位置0
    ImuStruct.yaw_T = 12;

    // pit_ms_init(1, imu_update);
 }
 



/******************************************* 运动积分应用区域 **************************************************************/
/**
  * @brief  偏航角积分开始
  * @param  None
  * @note   
  * @retval None
  */
void integral_yaw_start(void)
{
    ImuStruct.integral_yaw_flag = 1;       //开始积分
}


/**
  * @brief  偏航角积分
  * @param  None
  * @note   
  * @retval None
  */
 void integral_yaw_run(void)
 {
    if(ImuStruct.integral_yaw_flag)
    {
        counter_yaw ++;         
        if(counter_yaw == 1)    ImuStruct.yaw_start = ImuStruct.yaw;         // 记录开始积分时的yaw值
        else if(counter_yaw > 200)  counter_yaw = 200;              //其定义为uint8,超过一定数值会重新开始算，所以限幅

       if(fabs(ImuStruct.yaw - ImuStruct.yaw_start) > ImuStruct.yaw_T)         //如果偏航角累计大于某个阈值后
       {
             ImuStruct.yaw_end_flag = 1;                 //后续执行操作标志位置1
       }
    }
    else 
    {
        counter_yaw = 0;
    }
 }

 /**
  * @brief  偏航角积分结束
  * @param  None
  * @note   
  * @retval None
  */
void integral_yaw_stop(void)
{
    ImuStruct.integral_yaw_flag = 0; 
}
