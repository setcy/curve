/*
 *  Copyright (c) 2021 NetEase Inc.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/*
 * Project: Curve
 * Created Date: 2021-09-11
 * Author: Jingli Chen (Wine93)
 */

#ifndef CURVEFS_SRC_CLIENT_CLIENT_OPERATOR_H_
#define CURVEFS_SRC_CLIENT_CLIENT_OPERATOR_H_

#include <string>
#include <vector>
#include <memory>

#include "curvefs/src/client/inode_cache_manager.h"
#include "curvefs/src/client/dentry_cache_manager.h"
#include "curvefs/src/client/rpcclient/mds_client.h"

namespace curvefs {
namespace client {

using rpcclient::MdsClient;

class RenameOperator {
 public:
    RenameOperator(uint32_t fsId,
                   uint64_t parentId,
                   std::string name,
                   uint64_t newParentId,
                   std::string newname,
                   std::shared_ptr<DentryCacheManager> dentryManager,
                   std::shared_ptr<InodeCacheManager> inodeManager,
                   std::shared_ptr<MetaServerClient> metaClient,
                   std::shared_ptr<MdsClient> mdsClient);

    CURVEFS_ERROR GetTxId();
    CURVEFS_ERROR Precheck();
    CURVEFS_ERROR PrepareTx();
    CURVEFS_ERROR CommitTx();
    void UnlinkOldInode();
    void UpdateCache();

 private:
    std::string DebugString();

    CURVEFS_ERROR CheckOverwrite();

    CURVEFS_ERROR GetTxId(uint32_t fsId,
                          uint64_t inodeId,
                          uint32_t* partitionId,
                          uint64_t* txId);

    void SetTxId(uint32_t partitionId, uint64_t txId);

    CURVEFS_ERROR PrepareRenameTx(const std::vector<Dentry>& dentrys);

 private:
    uint32_t fsId_;
    uint64_t parentId_;
    std::string name_;
    uint64_t newParentId_;
    std::string newname_;

    uint32_t srcPartitionId_;
    uint32_t dstPartitionId_;
    uint64_t srcTxId_;
    uint64_t dstTxId_;
    uint64_t oldInodeId_;
    Dentry srcDentry_;
    Dentry dstDentry_;
    Dentry dentry_;
    Dentry newDentry_;

    std::shared_ptr<DentryCacheManager> dentryManager_;
    std::shared_ptr<InodeCacheManager> inodeManager_;
    std::shared_ptr<MetaServerClient> metaClient_;
    std::shared_ptr<MdsClient> mdsClient_;
};

}  // namespace client
}  // namespace curvefs

#endif  // CURVEFS_SRC_CLIENT_CLIENT_OPERATOR_H_