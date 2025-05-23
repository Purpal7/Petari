#include "Game/Camera/CameraAnim.hpp"
#include "Game/Camera/CamTranslatorAnim.hpp"

KeyCamAnmDataAccessor::~KeyCamAnmDataAccessor() {

}

void KeyCamAnmDataAccessor::set(void *pInfo, void *pValues) {
    mInfo = reinterpret_cast<CanmKeyFrameInfo *>(pInfo);
    mValues = reinterpret_cast<f32 *>(pValues);
}

void KeyCamAnmDataAccessor::getPos(TVec3f *pPos, f32 key) const {
    CanmKeyFrameComponentInfo &infoZ = mInfo->mPosZ;
    CanmKeyFrameComponentInfo &infoY = mInfo->mPosY;
    CanmKeyFrameComponentInfo &infoX = mInfo->mPosX;

    f32 x;
    f32 y;
    f32 z;

    z = get(key, infoZ.mOffset, infoZ.mCount, infoZ.mType);
    y = get(key, infoY.mOffset, infoY.mCount, infoY.mType);
    x = get(key, infoX.mOffset, infoX.mCount, infoX.mType);

    pPos->set(x, y, z);
}

void KeyCamAnmDataAccessor::getWatchPos(TVec3f *pWatchPos, f32 key) const {
    CanmKeyFrameComponentInfo &infoZ = mInfo->mWatchPosZ;
    CanmKeyFrameComponentInfo &infoY = mInfo->mWatchPosY;
    CanmKeyFrameComponentInfo &infoX = mInfo->mWatchPosX;

    f32 x;
    f32 y;
    f32 z;

    z = get(key, infoZ.mOffset, infoZ.mCount, infoZ.mType);
    y = get(key, infoY.mOffset, infoY.mCount, infoY.mType);
    x = get(key, infoX.mOffset, infoX.mCount, infoX.mType);

    pWatchPos->set(x, y, z);
}

f32 KeyCamAnmDataAccessor::getTwist(f32 key) const {
    CanmKeyFrameComponentInfo &info = mInfo->mTwist;
    
    return get(key, info.mOffset, info.mCount, info.mType);
}

f32 KeyCamAnmDataAccessor::getFovy(f32 key) const {
    CanmKeyFrameComponentInfo &info = mInfo->mFovy;
    
    return get(key, info.mOffset, info.mCount, info.mType);
}

f32 KeyCamAnmDataAccessor::get(f32 key, u32 offset, u32 count, u32 type) const {
    if (count == 1) {
        return mValues[offset];
    }

    if (type == 0) {
        return get3f(key, offset, count);
    }
    else {
        return get4f(key, offset, count);
    }
}

u32 KeyCamAnmDataAccessor::searchKeyFrameIndex(f32 key, u32 offset, u32 count, u32 stride) const {
    u32 low = 0;
    u32 high = count;

    while (low < high) {
        u32 middle = (low + high) / 2;

        if (mValues[offset + middle * stride] <= key) {
            low = middle + 1;
        }
        else {
            high = middle;
        }
    }

    return low - 1;
}

f32 KeyCamAnmDataAccessor::get3f(f32 key, u32 offset, u32 count) const {
    u32 index = searchKeyFrameIndex(key, offset, count, 3);
    f32 *values = mValues + offset + index * 3;

    return calcHermite(key, values[0], values[1], values[2], values[3], values[4], values[5]);
}

f32 KeyCamAnmDataAccessor::get4f(f32 key, u32 offset, u32 count) const {
    u32 index = searchKeyFrameIndex(key, offset, count, 4);
    f32 *values = mValues + offset + index * 4;

    return calcHermite(key, values[0], values[1], values[2], values[3], values[4], values[5]);
}

#ifdef NON_MATCHING
// Float instruction order, register mismatch
f32 KeyCamAnmDataAccessor::calcHermite(f32 key, f32 a2, f32 a3, f32 a4, f32 a5, f32 a6, f32 a7) const {
    f32 fVar1 = a4 / 30.0f;
    f32 fVar2 = (key - a2) / (a5 - a2);
    f32 fVar3 = fVar2 * fVar2 - fVar2;

    return -(key - a2) * (fVar2 * fVar1 - a7 / 30.0f * fVar3 + fVar1 * fVar3 + fVar1) - (((fVar2 + fVar2) * fVar3 - fVar2 * fVar2) * (a3 - a6) + a3);
}
#endif

CamAnmDataAccessor::~CamAnmDataAccessor() {
    
}

void CamAnmDataAccessor::set(void *pInfo, void *pValues) {
    mInfo = reinterpret_cast<CanmFrameInfo *>(pInfo);
    mValues = reinterpret_cast<f32 *>(pValues);
}

void CamAnmDataAccessor::getPos(TVec3f *pPos, f32 key) const {
    CamnFrameComponentInfo &infoZ = mInfo->mPosZ;
    CamnFrameComponentInfo &infoY = mInfo->mPosY;
    CamnFrameComponentInfo &infoX = mInfo->mPosX;

    f32 x;
    f32 y;
    f32 z;

    z = get(key, infoZ.mOffset, infoZ.mCount);
    y = get(key, infoY.mOffset, infoY.mCount);
    x = get(key, infoX.mOffset, infoX.mCount);

    pPos->set(x, y, z);
}

void CamAnmDataAccessor::getWatchPos(TVec3f *pWatchPos, f32 key) const {
    CamnFrameComponentInfo &infoZ = mInfo->mWatchPosZ;
    CamnFrameComponentInfo &infoY = mInfo->mWatchPosY;
    CamnFrameComponentInfo &infoX = mInfo->mWatchPosX;

    f32 x;
    f32 y;
    f32 z;

    z = get(key, infoZ.mOffset, infoZ.mCount);
    y = get(key, infoY.mOffset, infoY.mCount);
    x = get(key, infoX.mOffset, infoX.mCount);

    pWatchPos->set(x, y, z);
}

f32 CamAnmDataAccessor::getTwist(f32 key) const {
    CamnFrameComponentInfo &info = mInfo->mTwist;

    return get(key, info.mOffset, info.mCount);
}

f32 CamAnmDataAccessor::getFovy(f32 key) const {
    CamnFrameComponentInfo &info = mInfo->mFovy;

    return get(key, info.mOffset, info.mCount);
}

/*f32 CamAnmDataAccessor::get(f32 key, u32 offset, u32 count) const {
    u32 intKey = static_cast<u32>(key);
    f32 fKey = static_cast<f32>(intKey);

    f32 diff = key - fKey;

    if (diff < 0.0f) {
        
    }
}*/

CameraAnim::CameraAnim(const char *pName) : Camera(pName) {
    _4C = 0;
    _50 = 1;
    mNrFrames = 0;
    mIsKey = 0;
    mSpeed = 1.0f;
    mFileDataAccessor = nullptr;
    mDataAccessor = new CamAnmDataAccessor();
    mKeyDataAccessor = new KeyCamAnmDataAccessor();
    mNrValues = 0;
    _74 = 0;
    mFileData = nullptr;
    _7C = 0;
}

CameraAnim::~CameraAnim() {

}

bool CameraAnim::isZeroFrameMoveOff() const {
    return true;
}

bool CameraAnim::isCollisionOff() const {
    return true;
}

bool CameraAnim::isInterpolationOff() const {
    return true;
}

CamTranslatorBase *CameraAnim::createTranslator() {
    return new CamTranslatorAnim(this);
}

void CameraAnim::setParam(u8 *pFile, f32 speed) {
    loadBin(pFile);
    mFileData = pFile;
    mSpeed = speed;
}

bool CameraAnim::isAnimEnd() const {
    bool hasEnded = true;
    u32 nrFrames = mNrFrames;

    if (nrFrames != 0) {
        if (!(mCurrentFrame >= nrFrames)) {
            hasEnded = false;
        }
    }

    return hasEnded;
}

u32 CameraAnim::getAnimFrame(u8 *pFile) {
    if (pFile == nullptr) {
        return 0;
    }

    CanmFileHeader *pHeader = reinterpret_cast<CanmFileHeader *>(pFile);

    if (pHeader->mMagic[0] != 'A' || pHeader->mMagic[1] != 'N' || pHeader->mMagic[2] != 'D' || pHeader->mMagic[3] != 'O') {
        return 0;
    }

    if ((pHeader->mType[0] != 'C' || pHeader->mType[1] != 'A' || pHeader->mType[2] != 'N' || pHeader->mType[3] != 'M') &&
        (pHeader->mType[0] != 'C' || pHeader->mType[1] != 'K' || pHeader->mType[2] != 'A' || pHeader->mType[3] != 'N')) {
        return 0;
    }
    
    if (pHeader->_8 == 0) {
        return 0;
    }

    return pHeader->mNrFrames;
}

bool CameraAnim::loadBin(u8 *pFile) {
    CanmFileHeader *pHeader = reinterpret_cast<CanmFileHeader *>(pFile);

    if (pHeader->mMagic[0] != 'A' || pHeader->mMagic[1] != 'N' || pHeader->mMagic[2] != 'D' || pHeader->mMagic[3] != 'O') {
        return false;
    }

    if (pHeader->mType[0] == 'C' && pHeader->mType[1] == 'A' && pHeader->mType[2] == 'N' && pHeader->mType[3] == 'M') {
        mIsKey = 0;
        mFileDataAccessor = mDataAccessor;
    }
    else if (pHeader->mType[0] == 'C' && pHeader->mType[1] == 'K' && pHeader->mType[2] == 'A' && pHeader->mType[3] == 'N') {
        mIsKey = 1;
        mFileDataAccessor = mKeyDataAccessor;
    }
    else {
        return false;
    }

    if (pHeader->_8 == 0) {
        return false;
    }

    u8 *pEntry = reinterpret_cast<u8 *>(pFile + sizeof(CanmFileHeader));
    _4C = pHeader->_C;
    _50 = pHeader->_10;
    mNrFrames = pHeader->mNrFrames;

    u32 valueOffset = pHeader->mValueOffset;

    mNrValues = *(reinterpret_cast<u32 *>(&pEntry[valueOffset])) / sizeof(f32);
    
    mFileDataAccessor->set(pEntry, pEntry + valueOffset + 4);

    return true;
}