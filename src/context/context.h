#pragma once

#include <atomic>
#include <memory>
#include <thread>

#include "boost/asio/io_context.hpp"
#include "boost/asio/io_context_strand.hpp"

namespace buried {

class Context {
 public:
  static Context& GetGlobalContext() {
    static Context global_context;
    return global_context;
  }

  ~Context();

  using Strand = boost::asio::io_context::strand;
  using IOContext = boost::asio::io_context;

  Strand& GetMainStrand() { return main_strand_; }

  Strand& GetReportStrand() { return report_strand_; }

  IOContext& GetMainContext() { return main_context_; }

  void Start();

 private:
  Context() : main_strand_(main_context_), report_strand_(report_context_) {}

  Context(const Context&) = delete;
  Context& operator=(const Context&) = delete;

 private:
  boost::asio::io_context main_context_;
  boost::asio::io_context report_context_;

  boost::asio::io_context::strand main_strand_;
  boost::asio::io_context::strand report_strand_;

  std::unique_ptr<std::thread> main_thread_;
  std::unique_ptr<std::thread> report_thread_;

  std::atomic<bool> is_start_{false};
  std::atomic<bool> is_stop_{false};
};

}  // namespace buried
