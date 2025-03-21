#include <animation/propertyvaluesholder.h>
#include <porting/cdlog.h>

namespace cdroid{

PropertyValuesHolder::PropertyValuesHolder(){
    mProperty = nullptr;
    mValueType= Property::UNDEFINED;
}

PropertyValuesHolder::PropertyValuesHolder(const PropertyValuesHolder&o){
    mPropertyName = o.mPropertyName;
    mDataSource = o.mDataSource;
    mAnimateValue= o.mAnimateValue;
    mProperty = o.mProperty;
    mValueType= o.mValueType;
}

PropertyValuesHolder::PropertyValuesHolder(Property*property){
    mProperty = property;
    mValueType= property->getType();;
    if(property)mPropertyName = property->getName();
}

PropertyValuesHolder::PropertyValuesHolder(const std::string&name){
    mPropertyName = name;
    mValueType= Property::UNDEFINED;
    mProperty = nullptr;
}

PropertyValuesHolder::~PropertyValuesHolder(){
    //delete mProperty;
}

void PropertyValuesHolder::setPropertyName(const std::string& propertyName){
    mPropertyName = propertyName;
}

const std::string PropertyValuesHolder::getPropertyName()const{
    return mPropertyName;
}

void PropertyValuesHolder::setProperty(Property*p){
    mProperty = p;
    mValueType= p?p->getType():Property::UNDEFINED;
}

Property*PropertyValuesHolder::getProperty()const{
    return mProperty;
}

int PropertyValuesHolder::getValueType()const{
    return mValueType;
}

void PropertyValuesHolder::setPropertyChangedListener(const OnPropertyChangedListener&ls){
    mOnPropertyChangedListener = ls;
}

void PropertyValuesHolder::setupSetterAndGetter(void*target,const std::string&targetClass){
    if(mPropertyName.empty())return;
    if(mProperty==nullptr){
        mProperty = Property::fromName(targetClass,mPropertyName);
        mValueType= mProperty->getType();
    }
}

static int lerp(int startValue, int endValue, float fraction) {
    return int(startValue + std::round(fraction * (endValue - startValue)));
}

AnimateValue PropertyValuesHolder::evaluator(float fraction, const AnimateValue& from, const AnimateValue& to){
    AnimateValue out;
    switch(from.index()){
    case 0:
        out = (int)((1.f - fraction)*GET_VARIANT(from,int) +  fraction * GET_VARIANT(to,int));
        break;
    case 1:{
        float a = lerp((GET_VARIANT(from,uint32_t)>>24)/255.f,(GET_VARIANT(to,uint32_t)>>24)/255.f,fraction);
        float r = lerp(((GET_VARIANT(from,uint32_t)>>16)&0xFF)/255.f,((GET_VARIANT(to,uint32_t)>>16)&0xFF)/255.f,fraction);
        float g = lerp(((GET_VARIANT(from,uint32_t)>>8)&0xFF)/255.f,((GET_VARIANT(to,uint32_t)>>8)&0xFF)/255.f,fraction);
        float b = lerp((GET_VARIANT(from,uint32_t)&0xFF)/255.f,(GET_VARIANT(to,uint32_t)&0xFF)/255.f,fraction);
        out = ((uint32_t)(a*255.f)<<24)|((uint32_t)(r*255)<<16)|((uint32_t)(g*255)<<8)|((uint32_t)(b*255));
        }break;
    case 2:
        out = GET_VARIANT(from,float) * (1.f - fraction) + GET_VARIANT(to,float) * fraction;
        break;
    default:
        LOGE("NOT_REACHED");
    }
    return out;
}

void PropertyValuesHolder::setValues(const std::vector<int>&values){
    //mDataSource.resize(std::max(values.size(),size_t(2)));
    mDataSource.clear();
    mValueType = Property::INT_CLASS;
    for(size_t i=0;i<values.size();i++)
       mDataSource.push_back(values.at(i));
    mAnimateValue = values[0];
}

void PropertyValuesHolder::setValues(const std::vector<uint32_t>&values){
    //mDataSource.resize(std::max(values.size(),size_t(2)));
    mDataSource.clear();
    mValueType = Property::INT_CLASS;
    for(size_t i = 0;i < values.size();i++)
       mDataSource.push_back(values.at(i));
    mAnimateValue = values[0];
}

void PropertyValuesHolder::setValues(const std::vector<float>&values){
    //mDataSource.resize(std::max(values.size(),size_t(2)));
    mDataSource.clear();
    mValueType = Property::FLOAT_CLASS;
    for(size_t i = 0;i < values.size();i++)
       mDataSource.push_back(values.at(i));
    mAnimateValue = values[0];
}

void PropertyValuesHolder::init(){
    if(mEvaluator==nullptr){
        mEvaluator = evaluator;
    }
    mAnimateValue =mDataSource[0];
}

void PropertyValuesHolder::setEvaluator(TypeEvaluator evaluator){
    mEvaluator = evaluator;
}

void PropertyValuesHolder::calculateValue(float fraction){
    if (fraction <= 0.0f) mAnimateValue = mDataSource.front();
    else if (fraction >= 1.0f) mAnimateValue = mDataSource.back();
    else {
        fraction *= mDataSource.size() - 1;
        const int lowIndex = std::floor(fraction);
        fraction -= lowIndex;
        mAnimateValue = mEvaluator(fraction, mDataSource[lowIndex], mDataSource[lowIndex + 1]);
    } 
}

const AnimateValue& PropertyValuesHolder::getAnimatedValue()const{
    return mAnimateValue;
}

void PropertyValuesHolder::getPropertyValues(PropertyValues& values){
    init();
    values.propertyName = mPropertyName;
    values.type = mValueType;
    values.startValue=mDataSource[0];
    values.endValue=mDataSource[mDataSource.size()-1];
    LOGD("TODO property=%p %s",mProperty,mPropertyName.c_str());
#if 0
    values.startValue = mKeyframes.getValue(0);
    if (values.startValue instanceof PathParser::PathData) {
        // PathData evaluator returns the same mutable PathData object when query fraction,
        // so we have to make a copy here.
        values.startValue = new PathParser::PathData((PathParser::PathData) values.startValue);
    }
    values.endValue = mKeyframes.getValue(1);
    if (values.endValue instanceof PathParser::PathData) {
        // PathData evaluator returns the same mutable PathData object when query fraction,
        // so we have to make a copy here.
        values.endValue = new PathParser::PathData((PathParser::PathData) values.endValue);
    }
    // TODO: We need a better way to get data out of keyframes.
    if (mKeyframes instanceof PathKeyframes.FloatKeyframesBase
            || mKeyframes instanceof PathKeyframes.IntKeyframesBase
            || (mKeyframes.getKeyframes() != null && mKeyframes.getKeyframes().size() > 2)) {
        // When a pvh has more than 2 keyframes, that means there are intermediate values in
        // addition to start/end values defined for animators. Another case where such
        // intermediate values are defined is when animator has a path to animate along. In
        // these cases, a data source is needed to capture these intermediate values.
        values.getValueAtFraction=[this](float fraction)->AnimateValue {
            calculateValue(fraction);
            return mAnimateValue;
            //return mKeyframes.getValue(fraction);
        };
    } else {
        values.dataSource = nullptr;
    }
#endif
}

void PropertyValuesHolder::setAnimatedValue(void*target){
    if(mProperty!=nullptr){
        mProperty->set(target,getAnimatedValue());
    }else if(mSetter!=0){
        AnimateValue value = getAnimatedValue();
        mSetter(target,mPropertyName,value);
    }
}

void PropertyValuesHolder::setupValue(void*target,int position){
    if(mProperty){
        AnimateValue value = mProperty->get(target);
        if(mDataSource.size()==1)
            mDataSource.insert(mDataSource.begin()+position,value);
        else
            mDataSource[position] = value;
    }else if(mGetter){
        AnimateValue value = mGetter(target,mPropertyName);
        if(mDataSource.size()==1)
            mDataSource.insert(mDataSource.begin()+position,value);
        else
            mDataSource[position] = value;
    }
}

void PropertyValuesHolder::setupStartValue(void*target,const std::string&targetClass){
    if(!mDataSource.empty()){
        setupValue(target,0);
    }
}

void PropertyValuesHolder::setupEndValue(void*target,const std::string&targetClass){
    if(!mDataSource.empty()){
        setupValue(target,mDataSource.size()-1);
    }
}

PropertyValuesHolder* PropertyValuesHolder::ofInt(const std::string&name,const std::vector<int>&values){
    PropertyValuesHolder*pvh = new PropertyValuesHolder(name);
    pvh->mValueType=Property::INT_CLASS;
    pvh->setValues(values);
    return pvh;
}

PropertyValuesHolder* PropertyValuesHolder::ofInt(Property*prop,const std::vector<int>&values){
    PropertyValuesHolder*pvh = new PropertyValuesHolder(prop);
    pvh->mValueType=Property::INT_CLASS;
    pvh->setValues(values);
    return pvh;
}

PropertyValuesHolder* PropertyValuesHolder::ofFloat(const std::string&name,const std::vector<float>&values){
    PropertyValuesHolder*pvh = new PropertyValuesHolder(name);
    pvh->mValueType=Property::FLOAT_CLASS;
    pvh->setValues(values);
    return pvh;
}

PropertyValuesHolder* PropertyValuesHolder::ofFloat(Property*prop,const std::vector<float>&values){
    PropertyValuesHolder*pvh = new PropertyValuesHolder(prop);
    pvh->mValueType=Property::FLOAT_CLASS;
    pvh->setValues(values);
    return pvh;
}

PropertyValuesHolder*PropertyValuesHolder::ofObject(const std::string&propertyName,const std::vector<void*>&values){
    PropertyValuesHolder*pvh = new PropertyValuesHolder(propertyName);
    return pvh;
}

}//endof namespace
