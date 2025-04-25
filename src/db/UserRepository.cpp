#include "UserRepository.h"

// src/db/UserRepository.cpp
std::vector<User> UserRepository::getAll() {
  std::vector<User> users;
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(db, "SELECT id, name, email FROM users;", -1, &stmt, nullptr) == SQLITE_OK) {
      while (sqlite3_step(stmt) == SQLITE_ROW) {
          User user;
          user.id = sqlite3_column_int(stmt, 0);
          user.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
          user.email = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
          users.push_back(user);
      }
  }
  sqlite3_finalize(stmt);
  return users;
}

std::optional<User> UserRepository::findById(int id) {
  sqlite3_stmt* stmt;
  User user;
  if (sqlite3_prepare_v2(db, "SELECT name, email FROM users WHERE id = ?;", -1, &stmt, nullptr) == SQLITE_OK) {
      sqlite3_bind_int(stmt, 1, id);
      if (sqlite3_step(stmt) == SQLITE_ROW) {
          user.id = id;
          user.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
          user.email = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
          sqlite3_finalize(stmt);
          return user;
      }
  }
  sqlite3_finalize(stmt);
  return std::nullopt;
}

std::optional<User> UserRepository::findUserByName(const std::string& userName) {
  sqlite3_stmt* stmt;
  User user;
  if (sqlite3_prepare_v2(db, "SELECT id, name, email FROM users WHERE name = ?;", -1, &stmt, nullptr) == SQLITE_OK) {
      sqlite3_bind_text(stmt, 1, userName.c_str(), -1, SQLITE_TRANSIENT);
      if (sqlite3_step(stmt) == SQLITE_ROW) {
          user.id = sqlite3_column_int(stmt, 0);
          user.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
          user.email = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
          sqlite3_finalize(stmt);
          return user;
      }
  }
  sqlite3_finalize(stmt);
  return std::nullopt;
}

void UserRepository::save(const User& user) {
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(db, "INSERT INTO users (name, email) VALUES (?, ?);", -1, &stmt, nullptr) == SQLITE_OK) {
      sqlite3_bind_text(stmt, 1, user.name.c_str(), -1, SQLITE_TRANSIENT);
      sqlite3_bind_text(stmt, 2, user.email.c_str(), -1, SQLITE_TRANSIENT);
      sqlite3_step(stmt);
  }
  sqlite3_finalize(stmt);
}

void UserRepository::remove(int id) {
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(db, "DELETE FROM users WHERE id = ?;", -1, &stmt, nullptr) == SQLITE_OK) {
      sqlite3_bind_int(stmt, 1, id);
      sqlite3_step(stmt);
  }
  sqlite3_finalize(stmt);
}

void UserRepository::update(const User& user) {
  sqlite3_stmt* stmt;
  if (sqlite3_prepare_v2(db, "UPDATE users SET name = ?, email = ? WHERE id = ?;", -1, &stmt, nullptr) == SQLITE_OK) {
      sqlite3_bind_text(stmt, 1, user.name.c_str(), -1, SQLITE_TRANSIENT);
      sqlite3_bind_text(stmt, 2, user.email.c_str(), -1, SQLITE_TRANSIENT);
      sqlite3_bind_int(stmt, 3, user.id);
      sqlite3_step(stmt);
  }
  sqlite3_finalize(stmt);
}
