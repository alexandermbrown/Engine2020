// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_ASSETS_ASSETS_H_
#define FLATBUFFERS_GENERATED_ASSETS_ASSETS_H_

#include "flatbuffers/flatbuffers.h"

namespace Assets {

struct Vec2;

struct Vec3;

struct Vec4;

struct AssetBundle;
struct AssetBundleBuilder;

struct Texture2D;
struct Texture2DBuilder;

struct Shader;
struct ShaderBuilder;

struct AtlasEntry;
struct AtlasEntryBuilder;

struct TextureAtlas;
struct TextureAtlasBuilder;

struct GlyphEntry;
struct GlyphEntryBuilder;

struct Font;
struct FontBuilder;

struct Audio;
struct AudioBuilder;

struct LocaleEntry;
struct LocaleEntryBuilder;

struct Locale;
struct LocaleBuilder;

enum FilterType {
  FilterType_Error = 0,
  FilterType_Nearest = 1,
  FilterType_Linear = 2,
  FilterType_NearestMipmapNearest = 3,
  FilterType_LinearMipmapNearest = 4,
  FilterType_NearestMipmapLinear = 5,
  FilterType_MIN = FilterType_Error,
  FilterType_MAX = FilterType_NearestMipmapLinear
};

inline const FilterType (&EnumValuesFilterType())[6] {
  static const FilterType values[] = {
    FilterType_Error,
    FilterType_Nearest,
    FilterType_Linear,
    FilterType_NearestMipmapNearest,
    FilterType_LinearMipmapNearest,
    FilterType_NearestMipmapLinear
  };
  return values;
}

inline const char * const *EnumNamesFilterType() {
  static const char * const names[7] = {
    "Error",
    "Nearest",
    "Linear",
    "NearestMipmapNearest",
    "LinearMipmapNearest",
    "NearestMipmapLinear",
    nullptr
  };
  return names;
}

inline const char *EnumNameFilterType(FilterType e) {
  if (flatbuffers::IsOutRange(e, FilterType_Error, FilterType_NearestMipmapLinear)) return "";
  const size_t index = static_cast<size_t>(e);
  return EnumNamesFilterType()[index];
}

enum WrapType {
  WrapType_Error = 0,
  WrapType_Repeat = 1,
  WrapType_MirroredRepeat = 2,
  WrapType_ClampToEdge = 3,
  WrapType_ClampToBorder = 4,
  WrapType_MIN = WrapType_Error,
  WrapType_MAX = WrapType_ClampToBorder
};

inline const WrapType (&EnumValuesWrapType())[5] {
  static const WrapType values[] = {
    WrapType_Error,
    WrapType_Repeat,
    WrapType_MirroredRepeat,
    WrapType_ClampToEdge,
    WrapType_ClampToBorder
  };
  return values;
}

inline const char * const *EnumNamesWrapType() {
  static const char * const names[6] = {
    "Error",
    "Repeat",
    "MirroredRepeat",
    "ClampToEdge",
    "ClampToBorder",
    nullptr
  };
  return names;
}

inline const char *EnumNameWrapType(WrapType e) {
  if (flatbuffers::IsOutRange(e, WrapType_Error, WrapType_ClampToBorder)) return "";
  const size_t index = static_cast<size_t>(e);
  return EnumNamesWrapType()[index];
}

FLATBUFFERS_MANUALLY_ALIGNED_STRUCT(4) Vec2 FLATBUFFERS_FINAL_CLASS {
 private:
  float x_;
  float y_;

 public:
  Vec2() {
    memset(static_cast<void *>(this), 0, sizeof(Vec2));
  }
  Vec2(float _x, float _y)
      : x_(flatbuffers::EndianScalar(_x)),
        y_(flatbuffers::EndianScalar(_y)) {
  }
  float x() const {
    return flatbuffers::EndianScalar(x_);
  }
  float y() const {
    return flatbuffers::EndianScalar(y_);
  }
};
FLATBUFFERS_STRUCT_END(Vec2, 8);

FLATBUFFERS_MANUALLY_ALIGNED_STRUCT(4) Vec3 FLATBUFFERS_FINAL_CLASS {
 private:
  float x_;
  float y_;
  float z_;

 public:
  Vec3() {
    memset(static_cast<void *>(this), 0, sizeof(Vec3));
  }
  Vec3(float _x, float _y, float _z)
      : x_(flatbuffers::EndianScalar(_x)),
        y_(flatbuffers::EndianScalar(_y)),
        z_(flatbuffers::EndianScalar(_z)) {
  }
  float x() const {
    return flatbuffers::EndianScalar(x_);
  }
  float y() const {
    return flatbuffers::EndianScalar(y_);
  }
  float z() const {
    return flatbuffers::EndianScalar(z_);
  }
};
FLATBUFFERS_STRUCT_END(Vec3, 12);

FLATBUFFERS_MANUALLY_ALIGNED_STRUCT(4) Vec4 FLATBUFFERS_FINAL_CLASS {
 private:
  float x_;
  float y_;
  float z_;
  float w_;

 public:
  Vec4() {
    memset(static_cast<void *>(this), 0, sizeof(Vec4));
  }
  Vec4(float _x, float _y, float _z, float _w)
      : x_(flatbuffers::EndianScalar(_x)),
        y_(flatbuffers::EndianScalar(_y)),
        z_(flatbuffers::EndianScalar(_z)),
        w_(flatbuffers::EndianScalar(_w)) {
  }
  float x() const {
    return flatbuffers::EndianScalar(x_);
  }
  float y() const {
    return flatbuffers::EndianScalar(y_);
  }
  float z() const {
    return flatbuffers::EndianScalar(z_);
  }
  float w() const {
    return flatbuffers::EndianScalar(w_);
  }
};
FLATBUFFERS_STRUCT_END(Vec4, 16);

struct AssetBundle FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef AssetBundleBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_TEXTURES = 4,
    VT_SHADERS = 6,
    VT_ATLASES = 8,
    VT_FONTS = 10,
    VT_AUDIO = 12,
    VT_LOCALES = 14
  };
  const flatbuffers::Vector<flatbuffers::Offset<Assets::Texture2D>> *textures() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<Assets::Texture2D>> *>(VT_TEXTURES);
  }
  const flatbuffers::Vector<flatbuffers::Offset<Assets::Shader>> *shaders() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<Assets::Shader>> *>(VT_SHADERS);
  }
  const flatbuffers::Vector<flatbuffers::Offset<Assets::TextureAtlas>> *atlases() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<Assets::TextureAtlas>> *>(VT_ATLASES);
  }
  const flatbuffers::Vector<flatbuffers::Offset<Assets::Font>> *fonts() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<Assets::Font>> *>(VT_FONTS);
  }
  const flatbuffers::Vector<flatbuffers::Offset<Assets::Audio>> *audio() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<Assets::Audio>> *>(VT_AUDIO);
  }
  const flatbuffers::Vector<flatbuffers::Offset<Assets::Locale>> *locales() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<Assets::Locale>> *>(VT_LOCALES);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_TEXTURES) &&
           verifier.VerifyVector(textures()) &&
           verifier.VerifyVectorOfTables(textures()) &&
           VerifyOffset(verifier, VT_SHADERS) &&
           verifier.VerifyVector(shaders()) &&
           verifier.VerifyVectorOfTables(shaders()) &&
           VerifyOffset(verifier, VT_ATLASES) &&
           verifier.VerifyVector(atlases()) &&
           verifier.VerifyVectorOfTables(atlases()) &&
           VerifyOffset(verifier, VT_FONTS) &&
           verifier.VerifyVector(fonts()) &&
           verifier.VerifyVectorOfTables(fonts()) &&
           VerifyOffset(verifier, VT_AUDIO) &&
           verifier.VerifyVector(audio()) &&
           verifier.VerifyVectorOfTables(audio()) &&
           VerifyOffset(verifier, VT_LOCALES) &&
           verifier.VerifyVector(locales()) &&
           verifier.VerifyVectorOfTables(locales()) &&
           verifier.EndTable();
  }
};

struct AssetBundleBuilder {
  typedef AssetBundle Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_textures(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Assets::Texture2D>>> textures) {
    fbb_.AddOffset(AssetBundle::VT_TEXTURES, textures);
  }
  void add_shaders(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Assets::Shader>>> shaders) {
    fbb_.AddOffset(AssetBundle::VT_SHADERS, shaders);
  }
  void add_atlases(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Assets::TextureAtlas>>> atlases) {
    fbb_.AddOffset(AssetBundle::VT_ATLASES, atlases);
  }
  void add_fonts(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Assets::Font>>> fonts) {
    fbb_.AddOffset(AssetBundle::VT_FONTS, fonts);
  }
  void add_audio(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Assets::Audio>>> audio) {
    fbb_.AddOffset(AssetBundle::VT_AUDIO, audio);
  }
  void add_locales(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Assets::Locale>>> locales) {
    fbb_.AddOffset(AssetBundle::VT_LOCALES, locales);
  }
  explicit AssetBundleBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  AssetBundleBuilder &operator=(const AssetBundleBuilder &);
  flatbuffers::Offset<AssetBundle> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<AssetBundle>(end);
    return o;
  }
};

inline flatbuffers::Offset<AssetBundle> CreateAssetBundle(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Assets::Texture2D>>> textures = 0,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Assets::Shader>>> shaders = 0,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Assets::TextureAtlas>>> atlases = 0,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Assets::Font>>> fonts = 0,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Assets::Audio>>> audio = 0,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Assets::Locale>>> locales = 0) {
  AssetBundleBuilder builder_(_fbb);
  builder_.add_locales(locales);
  builder_.add_audio(audio);
  builder_.add_fonts(fonts);
  builder_.add_atlases(atlases);
  builder_.add_shaders(shaders);
  builder_.add_textures(textures);
  return builder_.Finish();
}

inline flatbuffers::Offset<AssetBundle> CreateAssetBundleDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const std::vector<flatbuffers::Offset<Assets::Texture2D>> *textures = nullptr,
    const std::vector<flatbuffers::Offset<Assets::Shader>> *shaders = nullptr,
    const std::vector<flatbuffers::Offset<Assets::TextureAtlas>> *atlases = nullptr,
    const std::vector<flatbuffers::Offset<Assets::Font>> *fonts = nullptr,
    const std::vector<flatbuffers::Offset<Assets::Audio>> *audio = nullptr,
    const std::vector<flatbuffers::Offset<Assets::Locale>> *locales = nullptr) {
  auto textures__ = textures ? _fbb.CreateVector<flatbuffers::Offset<Assets::Texture2D>>(*textures) : 0;
  auto shaders__ = shaders ? _fbb.CreateVector<flatbuffers::Offset<Assets::Shader>>(*shaders) : 0;
  auto atlases__ = atlases ? _fbb.CreateVector<flatbuffers::Offset<Assets::TextureAtlas>>(*atlases) : 0;
  auto fonts__ = fonts ? _fbb.CreateVector<flatbuffers::Offset<Assets::Font>>(*fonts) : 0;
  auto audio__ = audio ? _fbb.CreateVector<flatbuffers::Offset<Assets::Audio>>(*audio) : 0;
  auto locales__ = locales ? _fbb.CreateVector<flatbuffers::Offset<Assets::Locale>>(*locales) : 0;
  return Assets::CreateAssetBundle(
      _fbb,
      textures__,
      shaders__,
      atlases__,
      fonts__,
      audio__,
      locales__);
}

struct Texture2D FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef Texture2DBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_NAME = 4,
    VT_MIN_FILTER = 6,
    VT_MAG_FILTER = 8,
    VT_WRAP_S = 10,
    VT_WRAP_T = 12,
    VT_DATA = 14
  };
  const flatbuffers::String *name() const {
    return GetPointer<const flatbuffers::String *>(VT_NAME);
  }
  Assets::FilterType min_filter() const {
    return static_cast<Assets::FilterType>(GetField<int8_t>(VT_MIN_FILTER, 0));
  }
  Assets::FilterType mag_filter() const {
    return static_cast<Assets::FilterType>(GetField<int8_t>(VT_MAG_FILTER, 0));
  }
  Assets::WrapType wrap_s() const {
    return static_cast<Assets::WrapType>(GetField<int8_t>(VT_WRAP_S, 0));
  }
  Assets::WrapType wrap_t() const {
    return static_cast<Assets::WrapType>(GetField<int8_t>(VT_WRAP_T, 0));
  }
  const flatbuffers::Vector<uint8_t> *data() const {
    return GetPointer<const flatbuffers::Vector<uint8_t> *>(VT_DATA);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_NAME) &&
           verifier.VerifyString(name()) &&
           VerifyField<int8_t>(verifier, VT_MIN_FILTER) &&
           VerifyField<int8_t>(verifier, VT_MAG_FILTER) &&
           VerifyField<int8_t>(verifier, VT_WRAP_S) &&
           VerifyField<int8_t>(verifier, VT_WRAP_T) &&
           VerifyOffset(verifier, VT_DATA) &&
           verifier.VerifyVector(data()) &&
           verifier.EndTable();
  }
};

struct Texture2DBuilder {
  typedef Texture2D Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_name(flatbuffers::Offset<flatbuffers::String> name) {
    fbb_.AddOffset(Texture2D::VT_NAME, name);
  }
  void add_min_filter(Assets::FilterType min_filter) {
    fbb_.AddElement<int8_t>(Texture2D::VT_MIN_FILTER, static_cast<int8_t>(min_filter), 0);
  }
  void add_mag_filter(Assets::FilterType mag_filter) {
    fbb_.AddElement<int8_t>(Texture2D::VT_MAG_FILTER, static_cast<int8_t>(mag_filter), 0);
  }
  void add_wrap_s(Assets::WrapType wrap_s) {
    fbb_.AddElement<int8_t>(Texture2D::VT_WRAP_S, static_cast<int8_t>(wrap_s), 0);
  }
  void add_wrap_t(Assets::WrapType wrap_t) {
    fbb_.AddElement<int8_t>(Texture2D::VT_WRAP_T, static_cast<int8_t>(wrap_t), 0);
  }
  void add_data(flatbuffers::Offset<flatbuffers::Vector<uint8_t>> data) {
    fbb_.AddOffset(Texture2D::VT_DATA, data);
  }
  explicit Texture2DBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  Texture2DBuilder &operator=(const Texture2DBuilder &);
  flatbuffers::Offset<Texture2D> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<Texture2D>(end);
    return o;
  }
};

inline flatbuffers::Offset<Texture2D> CreateTexture2D(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::String> name = 0,
    Assets::FilterType min_filter = Assets::FilterType_Error,
    Assets::FilterType mag_filter = Assets::FilterType_Error,
    Assets::WrapType wrap_s = Assets::WrapType_Error,
    Assets::WrapType wrap_t = Assets::WrapType_Error,
    flatbuffers::Offset<flatbuffers::Vector<uint8_t>> data = 0) {
  Texture2DBuilder builder_(_fbb);
  builder_.add_data(data);
  builder_.add_name(name);
  builder_.add_wrap_t(wrap_t);
  builder_.add_wrap_s(wrap_s);
  builder_.add_mag_filter(mag_filter);
  builder_.add_min_filter(min_filter);
  return builder_.Finish();
}

inline flatbuffers::Offset<Texture2D> CreateTexture2DDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const char *name = nullptr,
    Assets::FilterType min_filter = Assets::FilterType_Error,
    Assets::FilterType mag_filter = Assets::FilterType_Error,
    Assets::WrapType wrap_s = Assets::WrapType_Error,
    Assets::WrapType wrap_t = Assets::WrapType_Error,
    const std::vector<uint8_t> *data = nullptr) {
  auto name__ = name ? _fbb.CreateString(name) : 0;
  auto data__ = data ? _fbb.CreateVector<uint8_t>(*data) : 0;
  return Assets::CreateTexture2D(
      _fbb,
      name__,
      min_filter,
      mag_filter,
      wrap_s,
      wrap_t,
      data__);
}

struct Shader FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef ShaderBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_NAME = 4,
    VT_GLSL = 6,
    VT_HLSL_VS = 8,
    VT_HLSL_PS = 10
  };
  const flatbuffers::String *name() const {
    return GetPointer<const flatbuffers::String *>(VT_NAME);
  }
  const flatbuffers::String *glsl() const {
    return GetPointer<const flatbuffers::String *>(VT_GLSL);
  }
  const flatbuffers::Vector<uint8_t> *hlsl_vs() const {
    return GetPointer<const flatbuffers::Vector<uint8_t> *>(VT_HLSL_VS);
  }
  const flatbuffers::Vector<uint8_t> *hlsl_ps() const {
    return GetPointer<const flatbuffers::Vector<uint8_t> *>(VT_HLSL_PS);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_NAME) &&
           verifier.VerifyString(name()) &&
           VerifyOffset(verifier, VT_GLSL) &&
           verifier.VerifyString(glsl()) &&
           VerifyOffset(verifier, VT_HLSL_VS) &&
           verifier.VerifyVector(hlsl_vs()) &&
           VerifyOffset(verifier, VT_HLSL_PS) &&
           verifier.VerifyVector(hlsl_ps()) &&
           verifier.EndTable();
  }
};

struct ShaderBuilder {
  typedef Shader Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_name(flatbuffers::Offset<flatbuffers::String> name) {
    fbb_.AddOffset(Shader::VT_NAME, name);
  }
  void add_glsl(flatbuffers::Offset<flatbuffers::String> glsl) {
    fbb_.AddOffset(Shader::VT_GLSL, glsl);
  }
  void add_hlsl_vs(flatbuffers::Offset<flatbuffers::Vector<uint8_t>> hlsl_vs) {
    fbb_.AddOffset(Shader::VT_HLSL_VS, hlsl_vs);
  }
  void add_hlsl_ps(flatbuffers::Offset<flatbuffers::Vector<uint8_t>> hlsl_ps) {
    fbb_.AddOffset(Shader::VT_HLSL_PS, hlsl_ps);
  }
  explicit ShaderBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ShaderBuilder &operator=(const ShaderBuilder &);
  flatbuffers::Offset<Shader> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<Shader>(end);
    return o;
  }
};

inline flatbuffers::Offset<Shader> CreateShader(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::String> name = 0,
    flatbuffers::Offset<flatbuffers::String> glsl = 0,
    flatbuffers::Offset<flatbuffers::Vector<uint8_t>> hlsl_vs = 0,
    flatbuffers::Offset<flatbuffers::Vector<uint8_t>> hlsl_ps = 0) {
  ShaderBuilder builder_(_fbb);
  builder_.add_hlsl_ps(hlsl_ps);
  builder_.add_hlsl_vs(hlsl_vs);
  builder_.add_glsl(glsl);
  builder_.add_name(name);
  return builder_.Finish();
}

inline flatbuffers::Offset<Shader> CreateShaderDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const char *name = nullptr,
    const char *glsl = nullptr,
    const std::vector<uint8_t> *hlsl_vs = nullptr,
    const std::vector<uint8_t> *hlsl_ps = nullptr) {
  auto name__ = name ? _fbb.CreateString(name) : 0;
  auto glsl__ = glsl ? _fbb.CreateString(glsl) : 0;
  auto hlsl_vs__ = hlsl_vs ? _fbb.CreateVector<uint8_t>(*hlsl_vs) : 0;
  auto hlsl_ps__ = hlsl_ps ? _fbb.CreateVector<uint8_t>(*hlsl_ps) : 0;
  return Assets::CreateShader(
      _fbb,
      name__,
      glsl__,
      hlsl_vs__,
      hlsl_ps__);
}

struct AtlasEntry FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef AtlasEntryBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_ALIAS = 4,
    VT_BOUNDS = 6
  };
  const flatbuffers::String *alias() const {
    return GetPointer<const flatbuffers::String *>(VT_ALIAS);
  }
  const Assets::Vec4 *bounds() const {
    return GetStruct<const Assets::Vec4 *>(VT_BOUNDS);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_ALIAS) &&
           verifier.VerifyString(alias()) &&
           VerifyField<Assets::Vec4>(verifier, VT_BOUNDS) &&
           verifier.EndTable();
  }
};

struct AtlasEntryBuilder {
  typedef AtlasEntry Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_alias(flatbuffers::Offset<flatbuffers::String> alias) {
    fbb_.AddOffset(AtlasEntry::VT_ALIAS, alias);
  }
  void add_bounds(const Assets::Vec4 *bounds) {
    fbb_.AddStruct(AtlasEntry::VT_BOUNDS, bounds);
  }
  explicit AtlasEntryBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  AtlasEntryBuilder &operator=(const AtlasEntryBuilder &);
  flatbuffers::Offset<AtlasEntry> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<AtlasEntry>(end);
    return o;
  }
};

inline flatbuffers::Offset<AtlasEntry> CreateAtlasEntry(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::String> alias = 0,
    const Assets::Vec4 *bounds = 0) {
  AtlasEntryBuilder builder_(_fbb);
  builder_.add_bounds(bounds);
  builder_.add_alias(alias);
  return builder_.Finish();
}

inline flatbuffers::Offset<AtlasEntry> CreateAtlasEntryDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const char *alias = nullptr,
    const Assets::Vec4 *bounds = 0) {
  auto alias__ = alias ? _fbb.CreateString(alias) : 0;
  return Assets::CreateAtlasEntry(
      _fbb,
      alias__,
      bounds);
}

struct TextureAtlas FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef TextureAtlasBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_NAME = 4,
    VT_TEXTURE = 6,
    VT_ENTRIES = 8
  };
  const flatbuffers::String *name() const {
    return GetPointer<const flatbuffers::String *>(VT_NAME);
  }
  const flatbuffers::String *texture() const {
    return GetPointer<const flatbuffers::String *>(VT_TEXTURE);
  }
  const flatbuffers::Vector<flatbuffers::Offset<Assets::AtlasEntry>> *entries() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<Assets::AtlasEntry>> *>(VT_ENTRIES);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_NAME) &&
           verifier.VerifyString(name()) &&
           VerifyOffset(verifier, VT_TEXTURE) &&
           verifier.VerifyString(texture()) &&
           VerifyOffset(verifier, VT_ENTRIES) &&
           verifier.VerifyVector(entries()) &&
           verifier.VerifyVectorOfTables(entries()) &&
           verifier.EndTable();
  }
};

struct TextureAtlasBuilder {
  typedef TextureAtlas Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_name(flatbuffers::Offset<flatbuffers::String> name) {
    fbb_.AddOffset(TextureAtlas::VT_NAME, name);
  }
  void add_texture(flatbuffers::Offset<flatbuffers::String> texture) {
    fbb_.AddOffset(TextureAtlas::VT_TEXTURE, texture);
  }
  void add_entries(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Assets::AtlasEntry>>> entries) {
    fbb_.AddOffset(TextureAtlas::VT_ENTRIES, entries);
  }
  explicit TextureAtlasBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  TextureAtlasBuilder &operator=(const TextureAtlasBuilder &);
  flatbuffers::Offset<TextureAtlas> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<TextureAtlas>(end);
    return o;
  }
};

inline flatbuffers::Offset<TextureAtlas> CreateTextureAtlas(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::String> name = 0,
    flatbuffers::Offset<flatbuffers::String> texture = 0,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Assets::AtlasEntry>>> entries = 0) {
  TextureAtlasBuilder builder_(_fbb);
  builder_.add_entries(entries);
  builder_.add_texture(texture);
  builder_.add_name(name);
  return builder_.Finish();
}

inline flatbuffers::Offset<TextureAtlas> CreateTextureAtlasDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const char *name = nullptr,
    const char *texture = nullptr,
    const std::vector<flatbuffers::Offset<Assets::AtlasEntry>> *entries = nullptr) {
  auto name__ = name ? _fbb.CreateString(name) : 0;
  auto texture__ = texture ? _fbb.CreateString(texture) : 0;
  auto entries__ = entries ? _fbb.CreateVector<flatbuffers::Offset<Assets::AtlasEntry>>(*entries) : 0;
  return Assets::CreateTextureAtlas(
      _fbb,
      name__,
      texture__,
      entries__);
}

struct GlyphEntry FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef GlyphEntryBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_CODEPOINT = 4,
    VT_TEXTURE_OFFSET = 6
  };
  uint32_t codepoint() const {
    return GetField<uint32_t>(VT_CODEPOINT, 0);
  }
  const Assets::Vec2 *texture_offset() const {
    return GetStruct<const Assets::Vec2 *>(VT_TEXTURE_OFFSET);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint32_t>(verifier, VT_CODEPOINT) &&
           VerifyField<Assets::Vec2>(verifier, VT_TEXTURE_OFFSET) &&
           verifier.EndTable();
  }
};

struct GlyphEntryBuilder {
  typedef GlyphEntry Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_codepoint(uint32_t codepoint) {
    fbb_.AddElement<uint32_t>(GlyphEntry::VT_CODEPOINT, codepoint, 0);
  }
  void add_texture_offset(const Assets::Vec2 *texture_offset) {
    fbb_.AddStruct(GlyphEntry::VT_TEXTURE_OFFSET, texture_offset);
  }
  explicit GlyphEntryBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  GlyphEntryBuilder &operator=(const GlyphEntryBuilder &);
  flatbuffers::Offset<GlyphEntry> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<GlyphEntry>(end);
    return o;
  }
};

inline flatbuffers::Offset<GlyphEntry> CreateGlyphEntry(
    flatbuffers::FlatBufferBuilder &_fbb,
    uint32_t codepoint = 0,
    const Assets::Vec2 *texture_offset = 0) {
  GlyphEntryBuilder builder_(_fbb);
  builder_.add_texture_offset(texture_offset);
  builder_.add_codepoint(codepoint);
  return builder_.Finish();
}

struct Font FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef FontBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_NAME = 4,
    VT_GLYPH_WIDTH = 6,
    VT_TEXTURE_WIDTH = 8,
    VT_DISTANCE_GRADIENT = 10,
    VT_GLYPHS = 12,
    VT_IMAGE = 14,
    VT_TTF = 16
  };
  const flatbuffers::String *name() const {
    return GetPointer<const flatbuffers::String *>(VT_NAME);
  }
  int16_t glyph_width() const {
    return GetField<int16_t>(VT_GLYPH_WIDTH, 0);
  }
  int16_t texture_width() const {
    return GetField<int16_t>(VT_TEXTURE_WIDTH, 0);
  }
  float distance_gradient() const {
    return GetField<float>(VT_DISTANCE_GRADIENT, 0.0f);
  }
  const flatbuffers::Vector<flatbuffers::Offset<Assets::GlyphEntry>> *glyphs() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<Assets::GlyphEntry>> *>(VT_GLYPHS);
  }
  const flatbuffers::Vector<uint8_t> *image() const {
    return GetPointer<const flatbuffers::Vector<uint8_t> *>(VT_IMAGE);
  }
  const flatbuffers::Vector<uint8_t> *ttf() const {
    return GetPointer<const flatbuffers::Vector<uint8_t> *>(VT_TTF);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_NAME) &&
           verifier.VerifyString(name()) &&
           VerifyField<int16_t>(verifier, VT_GLYPH_WIDTH) &&
           VerifyField<int16_t>(verifier, VT_TEXTURE_WIDTH) &&
           VerifyField<float>(verifier, VT_DISTANCE_GRADIENT) &&
           VerifyOffset(verifier, VT_GLYPHS) &&
           verifier.VerifyVector(glyphs()) &&
           verifier.VerifyVectorOfTables(glyphs()) &&
           VerifyOffset(verifier, VT_IMAGE) &&
           verifier.VerifyVector(image()) &&
           VerifyOffset(verifier, VT_TTF) &&
           verifier.VerifyVector(ttf()) &&
           verifier.EndTable();
  }
};

struct FontBuilder {
  typedef Font Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_name(flatbuffers::Offset<flatbuffers::String> name) {
    fbb_.AddOffset(Font::VT_NAME, name);
  }
  void add_glyph_width(int16_t glyph_width) {
    fbb_.AddElement<int16_t>(Font::VT_GLYPH_WIDTH, glyph_width, 0);
  }
  void add_texture_width(int16_t texture_width) {
    fbb_.AddElement<int16_t>(Font::VT_TEXTURE_WIDTH, texture_width, 0);
  }
  void add_distance_gradient(float distance_gradient) {
    fbb_.AddElement<float>(Font::VT_DISTANCE_GRADIENT, distance_gradient, 0.0f);
  }
  void add_glyphs(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Assets::GlyphEntry>>> glyphs) {
    fbb_.AddOffset(Font::VT_GLYPHS, glyphs);
  }
  void add_image(flatbuffers::Offset<flatbuffers::Vector<uint8_t>> image) {
    fbb_.AddOffset(Font::VT_IMAGE, image);
  }
  void add_ttf(flatbuffers::Offset<flatbuffers::Vector<uint8_t>> ttf) {
    fbb_.AddOffset(Font::VT_TTF, ttf);
  }
  explicit FontBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  FontBuilder &operator=(const FontBuilder &);
  flatbuffers::Offset<Font> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<Font>(end);
    return o;
  }
};

inline flatbuffers::Offset<Font> CreateFont(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::String> name = 0,
    int16_t glyph_width = 0,
    int16_t texture_width = 0,
    float distance_gradient = 0.0f,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Assets::GlyphEntry>>> glyphs = 0,
    flatbuffers::Offset<flatbuffers::Vector<uint8_t>> image = 0,
    flatbuffers::Offset<flatbuffers::Vector<uint8_t>> ttf = 0) {
  FontBuilder builder_(_fbb);
  builder_.add_ttf(ttf);
  builder_.add_image(image);
  builder_.add_glyphs(glyphs);
  builder_.add_distance_gradient(distance_gradient);
  builder_.add_name(name);
  builder_.add_texture_width(texture_width);
  builder_.add_glyph_width(glyph_width);
  return builder_.Finish();
}

inline flatbuffers::Offset<Font> CreateFontDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const char *name = nullptr,
    int16_t glyph_width = 0,
    int16_t texture_width = 0,
    float distance_gradient = 0.0f,
    const std::vector<flatbuffers::Offset<Assets::GlyphEntry>> *glyphs = nullptr,
    const std::vector<uint8_t> *image = nullptr,
    const std::vector<uint8_t> *ttf = nullptr) {
  auto name__ = name ? _fbb.CreateString(name) : 0;
  auto glyphs__ = glyphs ? _fbb.CreateVector<flatbuffers::Offset<Assets::GlyphEntry>>(*glyphs) : 0;
  auto image__ = image ? _fbb.CreateVector<uint8_t>(*image) : 0;
  auto ttf__ = ttf ? _fbb.CreateVector<uint8_t>(*ttf) : 0;
  return Assets::CreateFont(
      _fbb,
      name__,
      glyph_width,
      texture_width,
      distance_gradient,
      glyphs__,
      image__,
      ttf__);
}

struct Audio FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef AudioBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_NAME = 4,
    VT_DATA = 6
  };
  const flatbuffers::String *name() const {
    return GetPointer<const flatbuffers::String *>(VT_NAME);
  }
  const flatbuffers::Vector<uint8_t> *data() const {
    return GetPointer<const flatbuffers::Vector<uint8_t> *>(VT_DATA);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_NAME) &&
           verifier.VerifyString(name()) &&
           VerifyOffset(verifier, VT_DATA) &&
           verifier.VerifyVector(data()) &&
           verifier.EndTable();
  }
};

struct AudioBuilder {
  typedef Audio Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_name(flatbuffers::Offset<flatbuffers::String> name) {
    fbb_.AddOffset(Audio::VT_NAME, name);
  }
  void add_data(flatbuffers::Offset<flatbuffers::Vector<uint8_t>> data) {
    fbb_.AddOffset(Audio::VT_DATA, data);
  }
  explicit AudioBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  AudioBuilder &operator=(const AudioBuilder &);
  flatbuffers::Offset<Audio> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<Audio>(end);
    return o;
  }
};

inline flatbuffers::Offset<Audio> CreateAudio(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::String> name = 0,
    flatbuffers::Offset<flatbuffers::Vector<uint8_t>> data = 0) {
  AudioBuilder builder_(_fbb);
  builder_.add_data(data);
  builder_.add_name(name);
  return builder_.Finish();
}

inline flatbuffers::Offset<Audio> CreateAudioDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const char *name = nullptr,
    const std::vector<uint8_t> *data = nullptr) {
  auto name__ = name ? _fbb.CreateString(name) : 0;
  auto data__ = data ? _fbb.CreateVector<uint8_t>(*data) : 0;
  return Assets::CreateAudio(
      _fbb,
      name__,
      data__);
}

struct LocaleEntry FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef LocaleEntryBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_KEY = 4,
    VT_VALUE = 6
  };
  const flatbuffers::String *key() const {
    return GetPointer<const flatbuffers::String *>(VT_KEY);
  }
  const flatbuffers::Vector<int16_t> *value() const {
    return GetPointer<const flatbuffers::Vector<int16_t> *>(VT_VALUE);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_KEY) &&
           verifier.VerifyString(key()) &&
           VerifyOffset(verifier, VT_VALUE) &&
           verifier.VerifyVector(value()) &&
           verifier.EndTable();
  }
};

struct LocaleEntryBuilder {
  typedef LocaleEntry Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_key(flatbuffers::Offset<flatbuffers::String> key) {
    fbb_.AddOffset(LocaleEntry::VT_KEY, key);
  }
  void add_value(flatbuffers::Offset<flatbuffers::Vector<int16_t>> value) {
    fbb_.AddOffset(LocaleEntry::VT_VALUE, value);
  }
  explicit LocaleEntryBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  LocaleEntryBuilder &operator=(const LocaleEntryBuilder &);
  flatbuffers::Offset<LocaleEntry> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<LocaleEntry>(end);
    return o;
  }
};

inline flatbuffers::Offset<LocaleEntry> CreateLocaleEntry(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::String> key = 0,
    flatbuffers::Offset<flatbuffers::Vector<int16_t>> value = 0) {
  LocaleEntryBuilder builder_(_fbb);
  builder_.add_value(value);
  builder_.add_key(key);
  return builder_.Finish();
}

inline flatbuffers::Offset<LocaleEntry> CreateLocaleEntryDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const char *key = nullptr,
    const std::vector<int16_t> *value = nullptr) {
  auto key__ = key ? _fbb.CreateString(key) : 0;
  auto value__ = value ? _fbb.CreateVector<int16_t>(*value) : 0;
  return Assets::CreateLocaleEntry(
      _fbb,
      key__,
      value__);
}

struct Locale FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef LocaleBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_NAME = 4,
    VT_ENTRIES = 6
  };
  const flatbuffers::String *name() const {
    return GetPointer<const flatbuffers::String *>(VT_NAME);
  }
  const flatbuffers::Vector<flatbuffers::Offset<Assets::LocaleEntry>> *entries() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<Assets::LocaleEntry>> *>(VT_ENTRIES);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_NAME) &&
           verifier.VerifyString(name()) &&
           VerifyOffset(verifier, VT_ENTRIES) &&
           verifier.VerifyVector(entries()) &&
           verifier.VerifyVectorOfTables(entries()) &&
           verifier.EndTable();
  }
};

struct LocaleBuilder {
  typedef Locale Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_name(flatbuffers::Offset<flatbuffers::String> name) {
    fbb_.AddOffset(Locale::VT_NAME, name);
  }
  void add_entries(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Assets::LocaleEntry>>> entries) {
    fbb_.AddOffset(Locale::VT_ENTRIES, entries);
  }
  explicit LocaleBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  LocaleBuilder &operator=(const LocaleBuilder &);
  flatbuffers::Offset<Locale> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<Locale>(end);
    return o;
  }
};

inline flatbuffers::Offset<Locale> CreateLocale(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::String> name = 0,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<Assets::LocaleEntry>>> entries = 0) {
  LocaleBuilder builder_(_fbb);
  builder_.add_entries(entries);
  builder_.add_name(name);
  return builder_.Finish();
}

inline flatbuffers::Offset<Locale> CreateLocaleDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const char *name = nullptr,
    const std::vector<flatbuffers::Offset<Assets::LocaleEntry>> *entries = nullptr) {
  auto name__ = name ? _fbb.CreateString(name) : 0;
  auto entries__ = entries ? _fbb.CreateVector<flatbuffers::Offset<Assets::LocaleEntry>>(*entries) : 0;
  return Assets::CreateLocale(
      _fbb,
      name__,
      entries__);
}

inline const Assets::AssetBundle *GetAssetBundle(const void *buf) {
  return flatbuffers::GetRoot<Assets::AssetBundle>(buf);
}

inline const Assets::AssetBundle *GetSizePrefixedAssetBundle(const void *buf) {
  return flatbuffers::GetSizePrefixedRoot<Assets::AssetBundle>(buf);
}

inline const char *AssetBundleIdentifier() {
  return "LAB+";
}

inline bool AssetBundleBufferHasIdentifier(const void *buf) {
  return flatbuffers::BufferHasIdentifier(
      buf, AssetBundleIdentifier());
}

inline bool VerifyAssetBundleBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<Assets::AssetBundle>(AssetBundleIdentifier());
}

inline bool VerifySizePrefixedAssetBundleBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<Assets::AssetBundle>(AssetBundleIdentifier());
}

inline const char *AssetBundleExtension() {
  return "lab";
}

inline void FinishAssetBundleBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<Assets::AssetBundle> root) {
  fbb.Finish(root, AssetBundleIdentifier());
}

inline void FinishSizePrefixedAssetBundleBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<Assets::AssetBundle> root) {
  fbb.FinishSizePrefixed(root, AssetBundleIdentifier());
}

}  // namespace Assets

#endif  // FLATBUFFERS_GENERATED_ASSETS_ASSETS_H_
