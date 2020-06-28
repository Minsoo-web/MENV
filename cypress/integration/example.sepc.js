/// <reference types="cypress" />

context("CRUD", () => {
  it("CREATE", () => {
    cy.visit("http://127.0.0.1:8080/");
    cy.get("input.input").click().type("test type");
    cy.get(".button.is-info").click();
  });
});
