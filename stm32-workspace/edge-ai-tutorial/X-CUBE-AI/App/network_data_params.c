/**
  ******************************************************************************
  * @file    network_data_params.c
  * @author  AST Embedded Analytics Research Platform
  * @date    2026-10-11T22:21:02+0300
  * @brief   AI Tool Automatic Code Generator for Embedded NN computing
  ******************************************************************************
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  ******************************************************************************
  */

#include "network_data_params.h"


/**  Activations Section  ****************************************************/
ai_handle g_network_activations_table[1 + 2] = {
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
  AI_HANDLE_PTR(NULL),
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
};




/**  Weights Section  ********************************************************/
AI_ALIGNED(32)
const ai_u64 s_network_weights_array_u64[426] = {
  0xbf6826e040202045U, 0x401daa70bef0d115U, 0xbefe1f4dbf959ecfU, 0xbf80780d40052af1U,
  0x402492f8bed7e6f7U, 0xbeea5871bf2ae6bfU, 0xbf34493c40292ba9U, 0x3f859348bf191cc1U,
  0x3f1fba473fd5ba56U, 0x3fb92cab3eb93690U, 0x3e9e83033f6fc23bU, 0x3f8288423fad638eU,
  0x3fde1d33be5a324cU, 0xbe0d88993f64418dU, 0x3f90db333fca50a6U, 0xbfefdac4bec8b3a8U,
  0xbd5d94f1bfb4e389U, 0xbfdba9a4bfdeb05fU, 0xbfe8359b3e4a9b12U, 0x3f04360fbfccc315U,
  0xbfcb4706bfee21d3U, 0xbfebc7a73ef75e79U, 0x40036cdbbfd0da58U, 0xbf952802bf087bbdU,
  0xbf6495813fe1f869U, 0x3fe1ce0ebf70f0fbU, 0xbf7c5127bf52831fU, 0xbee5808d3fc6b0ccU,
  0x3fce5cd9bf662871U, 0xbf9e034ebea02dc3U, 0x3fa65a153ea8b166U, 0x3f00c3e23fb946c0U,
  0x3ff58d543f93b903U, 0x3fa1163a3c842cabU, 0x3ece29ea3ff08927U, 0x3fd495ca3f703602U,
  0x3f9d03943e672dc2U, 0xbd03577f4006ddabU, 0xbfc94c86bf986d78U, 0xbfac6987be4b5ee6U,
  0xbe06f4c4bff004bfU, 0xbfc34e65bf870548U, 0xbfbf91773e3670c5U, 0x3ed5e7eebffe0eacU,
  0xbffacf67bfa82838U, 0xbff4554cbf982ec2U, 0xbfc1babd3ff69d2cU, 0x3fcfb8c5bfe25b01U,
  0xbff4c086bf785fb0U, 0xbf88e7343fe3075cU, 0x3ffa7b76bfc0cf75U, 0xbff2c51abf6be9b8U,
  0x3f5794953fb9c002U, 0xbfffabe74019cfcaU, 0x4011e2653fa6de81U, 0x3f82f229c002e348U,
  0xbfcb7db340122393U, 0x4017f41b3ee5d1a0U, 0x3e5c467cbff81e53U, 0xbfff1b58400b098aU,
  0xbc9171a8bc92f719U, 0xbc0e32b43c8698aeU, 0x3c0d94c4bbcf741bU, 0x3f1b61323f122e2cU,
  0x3f848d8e3fe71161U, 0x3f7532593f931a0fU, 0x3f2facc33e9e8f99U, 0xbd7b61333d1d904bU,
  0x3f3fbcbb3fbacf32U, 0x3f86ad6a3f42075eU, 0x3f2157603d83ac98U, 0x3dbc8ef2be7110b2U,
  0x3f277ab33fe9b1f8U, 0x3f8514ab3f833040U, 0x3f2ec8f03e020adeU, 0x3e2ae933be1c7f82U,
  0x3f644d473fda2c2bU, 0x3f8c3fea3f7715d3U, 0x3f424db63e2b9332U, 0x3e1457a8bda34db0U,
  0x3f63b3303fbdc183U, 0x3facda563f97cb3aU, 0x3f4bff6c3eb24fc8U, 0xbe7a9222bdc367f8U,
  0xbfc81d70bfdefd07U, 0xbfebcad9bf58823cU, 0xbf6c53e8bfb726c3U, 0x3f2e9ab13ef1e47eU,
  0xbfe18648bfe68a39U, 0xbfc278f2bf887d4eU, 0xbf68fecabfb43375U, 0x3efea1293ed18205U,
  0xbfcfa7b9bfef73b4U, 0xbfdc7b21bf8d8a99U, 0xbf6832c2bfc6bb4dU, 0x3f2260af3f03a04cU,
  0xbfe824cebfccb53eU, 0xbfef7409bfa34e01U, 0xbf54e463bfc462e2U, 0x3eb678763efc28d1U,
  0xbfc23844bfe42ca1U, 0xbfd94a2bbfb1ddb4U, 0xbf971465bfa3c4eeU, 0x3ed648343f146ee9U,
  0xbfadfa603f846947U, 0x3fad5129bf1c6577U, 0x3f8c9e90be1c5728U, 0x3f7be7abbf95de57U,
  0xbfd706683f541b24U, 0x3fa2cd74be67ab92U, 0x3f6b12d3bf719b17U, 0x3f9c2f64bfb3a5c5U,
  0xbfc9f34f3f637878U, 0x3f8e64c6bd9532c2U, 0x3f8f6249bf9af3ecU, 0x3f96b42dbfb215ccU,
  0xbfe446043f3ab0feU, 0x3f9139ce3dd0e91aU, 0x3f89d06dbfb48320U, 0x3f9adf1cbfb270ceU,
  0xc012d7fd3f2d1f49U, 0x3fa1b0393e8b43ddU, 0x3fa31de3bfbf5625U, 0x3f9cae22bf9be2aeU,
  0xbfa173fa3f6ce674U, 0x3eb3b1a6bfe8db40U, 0xbfd05db1bfa8ddddU, 0x3f25f5cc3f22c4ffU,
  0xbfbbd2233f557215U, 0x3deb3552bfcbeca9U, 0xbff4f845bf86172eU, 0x3f393ee53f0a7cb7U,
  0xbf83f3703f50402dU, 0x3e79f8e7bfca80a1U, 0xbff138efbfb1ec55U, 0x3f3c11e23ee3dde6U,
  0xbfb5c0ff3f7fd1d7U, 0x3e5a4a0fbfd0373fU, 0xbfdeb0edbf8de386U, 0x3ecaa6743f1aea4fU,
  0xbf9d94133f6cf4f2U, 0x3d85ae32bfee59b8U, 0xbff07d35bfa19bf7U, 0x3f34bc7f3f26cd01U,
  0x3fca13c4bdadb558U, 0x3ee6e4f23fb11b20U, 0x3fea54d43fbb57aaU, 0xbdceb3e83d4f94b4U,
  0x3fc10383be9cb3e5U, 0x3e7548e73fdf6aa9U, 0x3fe21a573fb588e5U, 0xbe4b680bbe28776bU,
  0x3fa3210cbee352c4U, 0x3e88bc9e3fd828cfU, 0x3fd9d9313fad59c8U, 0xbd9cc4663deb0281U,
  0x3fc5e2ddbd7f6452U, 0x3e6866cd3fe7bf2cU, 0x3fc4cd8c3f9a7679U, 0xbd1cd22a3dfd5966U,
  0x3fb94765bd13cd5fU, 0x3e273e013fe38826U, 0x3fedcf663f87b2f8U, 0xbb844519bce00209U,
  0xbfabdc97bfd76230U, 0xbfbc21efbf9d1ba3U, 0xbf90875fbfc197eaU, 0x3f1896dd3ea738b2U,
  0xbfb0ef06bfb0a955U, 0xbfcb33adbf60cf5dU, 0xbf546696bfaabb4eU, 0x3ef5e3993efaedc7U,
  0xbfb90a2dbfae74d8U, 0xbfbc8a08bf9daed2U, 0xbf91d680bf9de597U, 0x3f0b20153f16e43bU,
  0xbfd80209bfc1a985U, 0xbfa061ebbf808c42U, 0xbf674dc9bfb7a7b9U, 0x3eabf1243ea7b1bfU,
  0xbfd1aac2bf9da520U, 0xbf9e1bffbf96542bU, 0xbfafcd06bfcc55d6U, 0x3f07e8763f1431b3U,
  0xbbf36e6a3d986b3cU, 0x3e1308a0be419c72U, 0xbe1796ae3e37bca4U, 0xbcec77863c7508f4U,
  0x3d87a6753c0921caU, 0x3da4f9e2be0cb7eaU, 0xbe1cae5bbe6b5ecaU, 0x3e08157dbda646c5U,
  0xbdd52ebfbdd0ea20U, 0xbe5b185fbd24ea7cU, 0xbe0c1625be6fb878U, 0x3d16d996bde549e6U,
  0x3e20cd5bbe5a0ee0U, 0x3cc2b12e3e1a8e09U, 0x3e1d5bdcbd981e6bU, 0xbd9ab9873d3deb9eU,
  0xbd0b4a3fbde4828cU, 0xbe7aba133dd24b59U, 0xbdb3c4033e3f1036U, 0xbe319871bdb89e8dU,
  0x3f894d77400c2cb1U, 0x3ff055723f44ee0fU, 0x3f7b25c03f5a42c4U, 0xbe6387343dcf02e6U,
  0x3f2c9d4940082537U, 0x3fe3c6613f7952c0U, 0x3f2f1bf83f716e89U, 0xbe8a7cec3e3ee00aU,
  0x3f5b204e40115cadU, 0x3ff73b363f1d948bU, 0x3f4ca6853f2c1341U, 0xbe996db33e60d214U,
  0x3f83294b400f4cdeU, 0x3fceacb83f3fe4c8U, 0x3f67e6433f34b506U, 0xbdc3168f3e793da7U,
  0x3f79a72f3ff90bf0U, 0x3fdd5e833f6b9cbdU, 0x3f83ca093f10224aU, 0x3c4f4c6a3eb6b64fU,
  0xbe3855c4be145f9bU, 0x3d902022bf469758U, 0xbf1285233f7694c7U, 0xbf3e56d43f97f7f4U,
  0xbda3f14abeb999cbU, 0xbe4620b0bf5a1b89U, 0xbf9ae0ac3fa7ff56U, 0xbf7a273e3f87cefaU,
  0x3e05cabcbf041f53U, 0xbeef20d7bfa604a5U, 0xbf8df33e3fb2b908U, 0xbf6baaaf3f5c1e45U,
  0x3e5dd8ddbf035ceeU, 0xbee88da4bf8d30a9U, 0xbf9f9b7f3fb6d43dU, 0xbeec35de3f6d6a20U,
  0x3e1ef90abea3f831U, 0xbf231ebabf975a82U, 0xbfab6ae53fac8f69U, 0xbee25d873f34f449U,
  0x3fe5f18fbf36db79U, 0xbdad01803fde79e6U, 0x3fbee4fb3fa5505cU, 0xbc989574be16aeefU,
  0x3fdcd4edbf399a68U, 0xbe9d80a93fd7c29cU, 0x3fbe70173fa1a0b8U, 0xbd9459763d9f20a7U,
  0x3fab1a1fbec33d31U, 0xbe2a1a803fac4586U, 0x3fe73d1a3fa8095cU, 0x3e23f9ec3de3c06cU,
  0x3fbb7d8dbeca0e23U, 0xbee50ed63fe2dae1U, 0x3fe98afc3f7ddedcU, 0x3e1a877fbddf5366U,
  0x3fae5424bf29dcd9U, 0xbe07f1b73fb76248U, 0x3ff71e253fa180e6U, 0x3e5ce2b7bd71d110U,
  0x3fafce5abfa93a02U, 0xbfab0634c0112654U, 0xc02391b83ed70186U, 0x3f9aae7dbf589dbaU,
  0x3fb875c8bfd748b1U, 0xbfc66893c0071136U, 0xc00ffac93e4b6dbeU, 0x3f901f55bf3a34abU,
  0x3f9949b4bffe1dbfU, 0xbfd8f0babff1ca66U, 0xc00c10ba3e9f3a47U, 0x3fa31326bef272dbU,
  0x3f8bdb05c004737aU, 0xc0036c5fbfec8d97U, 0xc01075643df8c086U, 0x3f98abd8bf442eaeU,
  0x3fa4da3ec0244749U, 0xc0060237bfcf58d2U, 0xc0049d7c3e09129fU, 0x3f64cef7bef564b9U,
  0x3f33eecc400228baU, 0x3fc104eb3f8c08d1U, 0x3fa23ba23f880591U, 0xbe049ea2bdc3c4bfU,
  0x3f2f75eb3ff66118U, 0x3fde811c3f8115f5U, 0x3f64ea0e3f616516U, 0xbd248aa13ded8072U,
  0x3f5d21c33feaaa9dU, 0x3fe02ee03f5d499dU, 0x3f8afe6c3f81132cU, 0xbcc5f0b73d372b8fU,
  0x3f25d2623fea359aU, 0x3fb89e3c3f84e84bU, 0x3f703ebf3f4820d4U, 0xbcfd227cbe1e0d3aU,
  0x3f20d3d53fe5d096U, 0x3fbb92c53f3dd45cU, 0x3f6690513f8325f0U, 0xbe262a473e3e2858U,
  0x3fe8c73dbf201e03U, 0xbdcf4a773fdd702dU, 0x3fc85e873fd117c2U, 0x3e614edfbdc805e0U,
  0x3feb2addbf10dfcaU, 0xbe90b3da3fe6710aU, 0x3feabfa33fb590ddU, 0x3e4b0bd3bdc545b2U,
  0x3fe2e494bed368f1U, 0xbe899af33fefe634U, 0x3fd0dc323fae7c32U, 0xbd8cbcdc3db801c9U,
  0x3fb3f8dcbf4108fbU, 0xbe94c25f3fd954afU, 0x3ffd8db53fa19a0aU, 0xbb6cf533be370b6bU,
  0x3fd1c8a3bf1d04c4U, 0xbcbf87c03fddc994U, 0x3ffc04ba3fb068a4U, 0xbc846e963da49027U,
  0x3cc6d75e3d1cbfa4U, 0xbe1115a43e436d7bU, 0x3e12605ebe08f6a6U, 0xbe696cadbe509102U,
  0xbdaf3cc73e2f2a7eU, 0x3df120d8be695e3aU, 0xbd8da9f1bdb58231U, 0xbe3a88ff3d5f7a52U,
  0x3e3ad497bda7e318U, 0xbd65d139bdac4658U, 0xbe26b83fbda8d408U, 0x3e24bc85bdad79dfU,
  0xbe23ed6b3d8938d8U, 0x3dfc1e7b3d191cdbU, 0xbe07b94cbdfe4830U, 0x3db0c7c23d655186U,
  0xbe4f2295be62f89fU, 0xbe37db23be5de90fU, 0x3d462b693dc024cfU, 0xba62ecd6be323f95U,
  0x3f3d27f5400b62bbU, 0x3fcd25853f1ccb2eU, 0x3f3aa7a03f3ab1b8U, 0x3e478b24bdf3351cU,
  0x3f7a53584003c980U, 0x3fcff5323f63fd59U, 0x3f4ad5b33f1814cfU, 0x3ead13783d87c6b1U,
  0x3f4a670a400497ccU, 0x3ff120313f839007U, 0x3f6794a23f3bcc51U, 0x3eaa45253df8d4c0U,
  0x3f3d833d400d348aU, 0x3ff5c70f3f82ee28U, 0x3f6cf4733ef26b96U, 0x3d5711e73e83d5adU,
  0x3f3dbc1c40070a35U, 0x3fc7f0393f1ffbd6U, 0x3f6f3a943f38a8b6U, 0xbcd8efc0bd544f9dU,
  0x3c8cad03be5fe598U, 0xbd40adb8bdecac54U, 0xbe54be4dbe590746U, 0x3d2e4d2fbdc72946U,
  0x3d5bf7f8bd42a320U, 0x3dd395edba41bccbU, 0x3d800419be2d4d51U, 0x3d57edf4be3249fcU,
  0xbdb8462f3e0c9b64U, 0xbd82b6c33d5a790fU, 0xbc2165d0bc3ea935U, 0xbd093a12bd038bbbU,
  0x3e4d3c75bdd71cd5U, 0xbe446c66bc6c5c90U, 0x3c02d716bda5313cU, 0xbdad8b6abe3847cbU,
  0xbe083805be5ada1eU, 0xbe391d493d316a99U, 0x3de9ef0ebd8fa046U, 0x3dce3bc43e1bb894U,
  0x3eba9947bbad14f6U, 0x3ec013093de3b538U, 0x3ea9decebd16564bU, 0xbd0c9701bcc0802eU,
  0x3d7ca6b43e2340efU, 0xbda5959e3e8cf1cfU, 0xbc8d66763b6f935bU, 0xbc8b9ca03d7291ecU,
  0xbfb75a963f8c962dU, 0xbf780738c01e2f97U, 0xbfa098323f293e26U, 0x3f7f94cdbec9e42bU,
  0x3f8a43dec01b5fa8U, 0x3f3a91943ff50dabU, 0xbe5aeb703f91c675U, 0x3edf91793f440336U,
  0x3fd175ebbfec8cd1U, 0x3f6b7607c02f9e57U, 0x3fbf8880bf9d3faaU, 0xc008fa7b3ea569c4U,
  0xbef945343ea52719U, 0xbffa75903fa008caU, 0x3dc3bc7dbddd7d29U, 0xbd548cbbbffca9f0U,
  0xbe7d35f33f015ebcU, 0x3f69499e3f71a63eU, 0x3c5ecf28bf3001e2U, 0x3f9622363e7e61d1U,
  0xc020a5963f106d9aU, 0x3f5bbc4fbffe4055U, 0xbe4662abc0340edaU, 0xbd72d2213f6c3f7cU,
  0xbfa7e716beaa28faU, 0xbfa5968d3fc41381U, 0xbfb1f3393f1df6dcU, 0x3ea8bc383da2bd66U,
  0x3f5a452a40050ecdU, 0x3edbe00cc0036c0bU, 0xbedc1f3b3f82d21dU, 0xbe84e8a73db433d4U,
  0xbdf58369bd517bb3U, 0xbdadcbf43e5a6603U,
};


ai_handle g_network_weights_table[1 + 2] = {
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
  AI_HANDLE_PTR(s_network_weights_array_u64),
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
};

