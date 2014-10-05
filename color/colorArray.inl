/** PackedColorArray implementations
**/

template<typename Color, size_t Elements>
typename PackedColorArray<Color, Elements>::value_type*
PackedColorArray<Color, Elements>::data()
{
  return data_.data();
}

template<typename Color, size_t Elements>
const typename PackedColorArray<Color, Elements>::value_type*
PackedColorArray<Color, Elements>::data() const
{
  return data_.data();
}

template<typename Color, size_t Elements>
void
PackedColorArray<Color, Elements>::clear()
{
  data_.fill(0);
}

template<typename Color, size_t Elements>
typename PackedColorArray<Color, Elements>::proxy
PackedColorArray<Color, Elements>::operator[](size_t i)
{
  size_t i_ = i*Bits/value_bitwidth;
  uint8_t offset = i * Bits % value_bitwidth;
  return proxy(data()[i_], offset);
}

template<typename Color, size_t Elements>
typename PackedColorArray<Color, Elements>::const_proxy
PackedColorArray<Color, Elements>::operator[](size_t i) const
{
  size_t i_ = i*Bits/value_bitwidth;
  uint8_t offset = i * Bits % value_bitwidth;
  return const_proxy(data()[i_], offset);
}

template<typename Color, size_t Elements>
typename PackedColorArray<Color, Elements>::iterator
PackedColorArray<Color, Elements>::begin()
{
  return iterator(data_.data(), 0);
}

template<typename Color, size_t Elements>
typename PackedColorArray<Color, Elements>::iterator
PackedColorArray<Color, Elements>::end()
{
  return iterator(data_.data(), Elements);
}

template<typename Color, size_t Elements>
typename PackedColorArray<Color, Elements>::const_iterator
PackedColorArray<Color, Elements>::begin() const
{
  return const_iterator(data(), 0);
}

template<typename Color, size_t Elements>
typename PackedColorArray<Color, Elements>::const_iterator
PackedColorArray<Color, Elements>::end() const
{
  return const_iterator(data_.data(), Elements);
}


/** PackedColorArray::proxyT implementations
**/

template<typename Color, size_t Elements>
template <bool isConst>
typename PackedColorArray<Color, Elements>::template proxyT<isConst>::value_type
PackedColorArray<Color, Elements>::proxyT<isConst>::read() const
{
  return (data_ & (arg_mask << offset_)) >> offset_;
}

template<typename Color, size_t Elements>
template <bool isConst>
PackedColorArray<Color, Elements>::proxyT<isConst>::proxyT(storage_type& data, size_t offset)
: data_(data), offset_(offset)
{
  Color::data_ = read();
}


/** PackedColorArray::proxy implementations
**/

template<typename Color, size_t Elements>
PackedColorArray<Color, Elements>::proxy::proxy(value_type& data, size_t offset)
  : proxyT<false>(data, offset)
{
}

template<typename Color, size_t Elements>
void
PackedColorArray<Color, Elements>::proxy::write(const value_type& value)
{
  value_type orVal = ((value & arg_mask) << proxyT<false>::offset_);
  proxyT<false>::data_ = (proxyT<false>::data_ & ~(arg_mask << proxyT<false>::offset_)) | orVal;
}

template<typename Color, size_t Elements>
template <typename C>
typename PackedColorArray<Color, Elements>::proxy&
PackedColorArray<Color, Elements>::proxy::operator=(const C& color)
{
  Color::operator=(color);
  return *this;
}

template<typename Color, size_t Elements>
typename PackedColorArray<Color, Elements>::proxy&
PackedColorArray<Color, Elements>::proxy::operator=(const proxy& other)
{
  write(other.read());
  return *this;
}

template<typename Color, size_t Elements>
PackedColorArray<Color, Elements>::proxy::~proxy()
{
  write(proxyT<false>::color_type::data_);
}


/** PackedColorArray::const_proxy implementations
**/

template<typename Color, size_t Elements>
PackedColorArray<Color, Elements>::const_proxy::const_proxy(const value_type& data, size_t offset)
 : proxyT<true>(data, offset)
{
}


/** PackedColorArray::iteratorT implementations
**/

template<typename Color, size_t Elements>
template <bool isConst>
PackedColorArray<Color, Elements>::iteratorT<isConst>::iteratorT(storage_type* data, size_t i)
  : data_(data), i_(i)
{
}

// Input iterator, Forward Iterator

template<typename Color, size_t Elements>
template <bool isConst>
typename PackedColorArray<Color, Elements>::template iteratorT<isConst>&
PackedColorArray<Color, Elements>::iteratorT<isConst>::operator++()
{
  i_++;
  return *this;
}

template<typename Color, size_t Elements>
template <bool isConst>
typename PackedColorArray<Color, Elements>::template iteratorT<isConst>
PackedColorArray<Color, Elements>::iteratorT<isConst>::operator++(int)
{
  iteratorT previous(*this);
  operator++();
  return previous;
}

template<typename Color, size_t Elements>
template <bool isConst>
template <bool b>
bool
PackedColorArray<Color, Elements>::iteratorT<isConst>::operator==(const PackedColorArray<Color, Elements>::iteratorT<b>& rhs) const
{
  return ((data_ == rhs.data_) && (i_ == rhs.i_));
}

template<typename Color, size_t Elements>
template <bool isConst>
template <bool b>
bool
PackedColorArray<Color, Elements>::iteratorT<isConst>::operator!=(const PackedColorArray<Color, Elements>::iteratorT<b>& rhs) const
{
  return !(operator==(rhs));
}

template<typename Color, size_t Elements>
template <bool isConst>
typename PackedColorArray<Color, Elements>::template iteratorT<isConst>::value_type
PackedColorArray<Color, Elements>::iteratorT<isConst>::operator*() const
{
  size_t dataIndex = (i_*Bits)/value_bitwidth;
  uint8_t offset = (i_ * Bits) % value_bitwidth;
  return value_type(data_[dataIndex], offset);
}

// Bidirectional iterator

template<typename Color, size_t Elements>
template <bool isConst>
typename PackedColorArray<Color, Elements>::template iteratorT<isConst>&
PackedColorArray<Color, Elements>::iteratorT<isConst>::operator--()
{
  i_--;
}

template<typename Color, size_t Elements>
template <bool isConst>
typename PackedColorArray<Color, Elements>::template iteratorT<isConst>
PackedColorArray<Color, Elements>::iteratorT<isConst>::operator--(int)
{
  iteratorT previous(*this);
  operator--();
  return previous;
}

// Random Access Iterator

template<typename Color, size_t Elements>
template <bool isConst>
typename PackedColorArray<Color, Elements>::template iteratorT<isConst>&
PackedColorArray<Color, Elements>::iteratorT<isConst>::operator+=(size_t n)
{
  i_ += n;
  return *this;
}

template<typename Color, size_t Elements>
template <bool isConst>
typename PackedColorArray<Color, Elements>::template iteratorT<isConst>
PackedColorArray<Color, Elements>::iteratorT<isConst>::operator+(size_t n) const
{
  iteratorT result(*this);
  result+= n;
  return result;
}

template<typename Color, size_t Elements>
template <bool isConst>
typename PackedColorArray<Color, Elements>::template iteratorT<isConst>&
PackedColorArray<Color, Elements>::iteratorT<isConst>::operator-=(size_t n)
{
  i_ -= n;
  return *this;
}

template<typename Color, size_t Elements>
template <bool isConst>
typename PackedColorArray<Color, Elements>::template iteratorT<isConst>
PackedColorArray<Color, Elements>::iteratorT<isConst>::operator-(size_t n) const
{
  iteratorT result(*this);
  result-= n;
  return result;
}

// Difference (do I need further comparisons?)

template<typename Color, size_t Elements>
template <bool isConst>
typename PackedColorArray<Color, Elements>::template iteratorT<isConst>::difference_type
PackedColorArray<Color, Elements>::iteratorT<isConst>::operator-(const iteratorT& rhs) const
{
  return i_ - rhs.i_;
}
