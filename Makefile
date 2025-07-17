SRCDIR := src
INCDIR := include
BUILDDIR := build
BINDIR := bin

TARGET := $(BINDIR)/main
LDFLAGS := -lncursesw

CXX := g++
CXXFLAGS := -Wall -MMD -I$(INCDIR) -std=c++20
MAIN_CXXFLAGS := -D_XOPEN_SOURCE_EXTENDED

# cppファイル取得
SRCS := $(shell find $(SRCDIR) -name '*.cpp')

# cppからoへ変換
OBJS := $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(SRCS))

# 依存ファイル生成
DEPS := $(OBJS:.o=.d)

all: $(TARGET)

# リンク
$(TARGET): $(OBJS) | $(BINDIR)
	$(CXX) -o $@ $^ $(LDFLAGS) $(MAIN_CXXFLAGS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp | $(BUILDDIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# ディレクトリ生成
$(BUILDDIR) $(BINDIR):
	mkdir -p $@

-include $(DEPS)

clean:
	rm -rf $(BUILDDIR) $(BINDIR)
